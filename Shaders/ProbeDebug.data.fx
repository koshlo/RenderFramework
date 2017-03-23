#define SHADER_DATA_NAME ProbeDebugShaderData

#define SHADER_PARAM_LIST \
TEXTURE_CUBE(CubeMap, float4); \
SAMPLER_STATE(CubeMapSampler); \
CONSTANT(TranslationScale, float4);

#include "../ShaderDataGen.h"