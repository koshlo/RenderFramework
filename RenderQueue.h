#ifndef _RENDERER_QUEUE_H_
#define _RENDERER_QUEUE_H_

#include "DrawCall.h"
#include "Imaging/Image.h"
#include <vector>

typedef void* RenderCommand;
struct RenderState;
typedef const RenderState* RenderStatePtr;

struct DrawCallState
{
	RenderStatePtr renderState;
	ShaderData* shaderData;
};

struct DispatchGroup
{
	uint x;
	uint y;
	uint z;
};

class RenderQueue
{
public:
	typedef uint32_t SortKeyType;

	RenderQueue(GraphicsDevice* gfxDevice, uint rtWidth, uint rtHeight, uint rtCount, FORMAT rtFormat, FORMAT depthFormat);

	void SetClear(bool clearRT, bool clearDepth, float4 clearColor, float depthClearVal);
	void AddShaderData(ShaderData* shaderData);

	template<typename T>
	T& AddRenderCommand(const SortKeyType& sortKey, const DrawCallState& drawState)
	{
		static_assert(std::is_standard_layout<T>(), "Render command should have standard layout");

		RenderFunc renderFunc = T::Render;
		void* data = AddDrawCommand(sizeof(T), renderFunc, drawState);
		_sortingKeys[_currentCommand] = sortKey;
		T* commandData = static_cast<T*>(data);
		return *commandData;
	}

	void Sort();
	void SubmitAll(GraphicsDevice* gfxDevice, StateHelper* stateHelper);

	TextureID GetRenderTarget(uint index) const;
	TextureID GetDepthTarget() const;

	static void DispatchCompute(StateHelper* stateHelper, const DispatchGroup& group, ShaderID shader, ShaderData** shaderData, uint numShaderData);

private:
	typedef std::pair<SortKeyType, RenderCommand> SortKeyPair;

	uint GetAlignedOffset(uint size, uint alignment)
	{
		ASSERT(alignment != 0);

		uint alignMask = ~(alignment - 1);
		return (size + alignment - 1) & alignMask;
	}

	void* AddDrawCommand(uint dataSize, RenderFunc renderFunc, const DrawCallState& drawState);

	static const uint MaxCommands = 4096;
	static const uint BufferSize = MaxCommands * 64;
	static const uint MaxShaderDataPerQueue = 8;
	static const uint MaxRenderTargets = 5;

	std::vector<uint8> _buffer;
	RenderCommand _commands[MaxCommands];
	SortKeyType _sortingKeys[MaxCommands];
	uint _currentCommand;

	ShaderData* _shaderData[MaxShaderDataPerQueue];
	uint _numShaderData;
	TextureID _renderTargets[MaxRenderTargets];
	uint _numRenderTargets;
	TextureID _depthRT;

	bool _clearRT;
	bool _clearDepth;
	float4 _clearColor;
	float _depthClearVal;
};

#endif // _RENDERER_QUEUE_H_