#define SHADER_DATA_NAME BlitShaderData

#define SHADER_PARAM_LIST \
TEXTURE_2D(SourceTexture, float4); \
SAMPLER_STATE(SourceSampler);

#include "../ShaderDataGen.h"