#define SHADER_DATA_NAME ConvolveEnvMapShaderData

#define NUM_THREADS 32
#define NUM_FACES 6

#define SHADER_PARAM_LIST \
TEXTURE_CUBE(EnvironmentMap, float4); \
SAMPLER_STATE(EnvMapSampler); \
CONSTANT(Normal, float3); \
CONSTANT(Up, float3); \
CONSTANT(Right, float3); \
CONSTANT(Resolution, float2); \
CONSTANT(FrameRandom, float2); \
CONSTANT(Face, uint); \
UAV_TEXTURE_2D_ARRAY(IrradianceCubeMapUAV, float4);

#include "../ShaderDataGen.h"