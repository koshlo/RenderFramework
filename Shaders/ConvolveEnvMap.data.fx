#define SHADER_DATA_NAME ConvolveEnvMapShaderData

#define SHADER_PARAM_LIST \
TEXTURE_CUBE(EnvironmentMap, float4); \
SAMPLER_STATE(EnvMapSampler); \
CONSTANT(Normal, float4); \
UAV_TEXTURE_2D(IrradianceUAV, float4);

#include "../ShaderDataGen.h"