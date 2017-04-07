#define SHADER_DATA_NAME LightShaderData

#define SHADER_PARAM_LIST \
CONSTANT(SunDirection, float3); \
CONSTANT(SunIntensity, float3); \
TEXTURE_CUBE_ARRAY(IrradianceProbes, float4); \
SAMPLER_STATE(IrradianceSampler); \
CONSTANT_ARRAY(ProbeLocations, float4, 16); \
CONSTANT(ProbeCount, uint); \
SAMPLER_STATE(MaterialSampler);

#include "../ShaderDataGen.h"