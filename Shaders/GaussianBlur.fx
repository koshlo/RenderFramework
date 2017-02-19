#include "Shadow.inc.fx"
#include "GaussianBlur.data.fx"

#define THREAD_COUNT_X 	16
#define THREAD_COUNT_Y 	16
#define CACHE_SIZE_X	THREAD_COUNT_X + MAX_BLUR_SIZE * 2
#define CACHE_SIZE_Y	THREAD_COUNT_Y + MAX_BLUR_SIZE * 2

groupshared uint TextureCache[CACHE_SIZE_X][CACHE_SIZE_Y];

static float2 TextureSize = SampleParams.xy;
static float2 SampleStep = SampleParams.zw;
static float WeightsPacked[MAX_BLUR_SIZE] = (float[MAX_BLUR_SIZE])Weights;
static uint2 MaxThreads = uint2(THREAD_COUNT_X, THREAD_COUNT_Y);
static uint2 NumScatteredCacheWrites = ( uint2(BlurHalfSize, BlurHalfSize) * 2 - 1 ) / MaxThreads + 1;
static uint2 CacheOffset = uint2(MAX_BLUR_SIZE, MAX_BLUR_SIZE);

uint PackFloat2(float2 value)
{
	float unwarpedX = UnwarpDepth(value.x);
	float unwarpedY = UnwarpDepth(value.y);
	uint lo = f32tof16(unwarpedX);
	uint hi = f32tof16(unwarpedY);
	return lo | (hi << 16);
}

float2 UnpackFloat2(uint packedValue)
{
	float x = WarpDepth(f16tof32(packedValue));
	float y = WarpDepth(f16tof32(packedValue >> 16));
	return float2(x, y);
}

uint2 GetCacheCoords(int2 vec)
{
	return vec.xy + CacheOffset;
}

int2 clamp0(int2 value)
{
	return max(0, value);
}

int2 ClampCoords(int2 coords)
{
	return clamp(coords, 0, TextureSize);
}

void DebugInitCache()
{
	for (uint i = 0; i < CACHE_SIZE_X; ++i)
	{
		for (uint j = 0; j < CACHE_SIZE_Y; ++j)
		{
			TextureCache[i][j] = PackFloat2(float2(0, 0));
		}
	}
}

void GatherTextureCache(uint3 threadId, uint3 groupThreadId, uint3 groupId, uint groupIndex)
{
	uint2 cacheCoords = GetCacheCoords(groupThreadId.xy);
	TextureCache[cacheCoords.x][cacheCoords.y] = PackFloat2(SourceTexture[threadId.xy]);

	int2 additionalReadsSouth = (groupThreadId.xy == 0) * BlurHalfSize * SampleStep;
	int2 additionalReadsNorth = (groupThreadId.xy == MaxThreads-1) * BlurHalfSize * SampleStep;

	for (int i = 1; i <= additionalReadsSouth.x; i++)
	{
		uint2 addCacheCoords = GetCacheCoords(int2(-i, groupThreadId.y));
		TextureCache[addCacheCoords.x][addCacheCoords.y] = PackFloat2(SourceTexture[ClampCoords(threadId.xy - int2(i, 0))]);
	}
	for (int i = 0; i < additionalReadsNorth.x; ++i)
	{
		uint2 addCacheCoords = GetCacheCoords(int2(MaxThreads.x + i, groupThreadId.y));
		TextureCache[addCacheCoords.x][addCacheCoords.y] = PackFloat2(SourceTexture[ClampCoords(threadId.xy + int2(i+1, 0))]);
	}
	for (int i = 1; i <= additionalReadsSouth.y; ++i)
	{
		uint2 addCacheCoords = GetCacheCoords(int2(groupThreadId.x, -i));
		TextureCache[addCacheCoords.x][addCacheCoords.y] = PackFloat2(SourceTexture[ClampCoords(threadId.xy - int2(0, i))]);
	}
	for (int i = 0; i < additionalReadsNorth.y; ++i)
	{
		uint2 addCacheCoords = GetCacheCoords(int2(groupThreadId.x, MaxThreads.y + i));
		TextureCache[addCacheCoords.x][addCacheCoords.y] = PackFloat2(SourceTexture[ClampCoords(threadId.xy + int2(0, i+1))]);
	}
}

/*
void GatherTextureCacheScattered(uint3 threadId, uint3 groupThreadId, uint3 groupId, uint groupIndex)
{
	float2 cacheCoords = GetCacheCoords(groupThreadId);
	
	uint2 groupOrigin = threadId.xy - groupThreadId.xy;
	uint2 groupEnd = groupOrigin + MaxThreads - 1;

	uint2 localOrigin = GetCacheCoords(0);
	uint2 localEnd = GetCacheCoords(MaxThreads - 1);

	TextureCache[cacheCoords.x][cacheCoords.y] = PackFloat2(SourceTexture[threadId.xy]);
	
	int2 additionalWrites = NumScatteredCacheWrites * SampleStep;	

	int2 readFromRight = groupThreadId % 2;
	int2 diffSign = readFromRight * 2 - 1;
	int2 sourceCoord = lerp(groupOrigin, groupEnd, readFromRight);
	int2 cacheWriteCoord = lerp(localOrigin, localEnd, readFromRight);

	for (int horIndex = 1; horIndex <= additionalWrites.x; horIndex++)
	{
		int i = ( groupThreadId.x + (horIndex-1) * THREAD_COUNT_X ) % (BlurHalfSize * 2); 		
		int diff = (i * additionalWrites.x + horIndex) * diffSign.x % (int)(BlurHalfSize + 1);
		TextureCache[cacheWriteCoord.x + diff][groupThreadId.y + CacheOffset.y] = PackFloat2(SourceTexture[float2(sourceCoord.x + diff, threadId.y)]);
	}
	for (int vertIndex = 1; vertIndex <= additionalWrites.y; vertIndex++)
	{
		int i = groupThreadId.y % BlurHalfSize;		
		int diff = (i * additionalWrites.y + vertIndex) * diffSign.y % (int)(BlurHalfSize + 1);
		TextureCache[groupThreadId.x + CacheOffset.x][cacheWriteCoord.y + diff] = PackFloat2(SourceTexture[float2(threadId.x, sourceCoord.y + diff)]);
	}
}
*/

float2 BlurPixel(uint2 coords, float weight)
{
    float2 destPixel = UnpackFloat2(TextureCache[coords.x][coords.y]);
    return destPixel * weight;
}

void PerformBlur(uint3 threadId, uint3 groupThreadId)
{
    uint2 coords = GetCacheCoords(groupThreadId);
    float2 result = BlurPixel(coords, WeightsPacked[0]);
    for (uint i = 1; i <= BlurHalfSize; i++)
    {
        uint2 leftCoords = coords - SampleStep * i;
        result += BlurPixel(leftCoords, WeightsPacked[i]);
        uint2 rightCoords = coords + SampleStep * i;
        result += BlurPixel(rightCoords, WeightsPacked[i]);
    }

    DestTexture[threadId.xy] = result;
}

[numthreads(THREAD_COUNT_X, THREAD_COUNT_Y, 1)]
void main(uint3 threadId : SV_DispatchThreadID, uint3 groupThreadId : SV_GroupThreadID, uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
	GatherTextureCache(threadId, groupThreadId, groupId, groupIndex);
	GroupMemoryBarrierWithGroupSync();
	PerformBlur(threadId, groupThreadId);
}