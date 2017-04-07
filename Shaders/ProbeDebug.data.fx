#define SHADER_DATA_NAME ProbeDebugShaderData

#define SHADER_PARAM_LIST \
TEXTURE_CUBE_ARRAY(CubeMapArray, float4); \
SAMPLER_STATE(CubeMapSampler); \
CONSTANT(ProbeIndex, uint); \
CONSTANT(TranslationScale, float4);

#include "../ShaderDataGen.h"