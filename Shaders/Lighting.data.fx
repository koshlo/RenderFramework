#define SHADER_DATA_NAME LightShaderData

#define SHADER_PARAM_LIST \
CONSTANT(SunDirection, float3); \
CONSTANT(SunIntensity, float3); \
CONSTANT(Ambient, float3); \
SAMPLER_STATE(MaterialSampler);

#include "../ShaderDataGen.h"