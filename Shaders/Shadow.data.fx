#define SHADER_DATA_NAME ShadowShaderData

#define SHADER_PARAM_LIST \
SAMPLER_STATE(VarianceSampler); \
TEXTURE_2D(ShadowMap, float2); \
CONSTANT(ShadowMapProjection, float4x4); \
CONSTANT(ExpPower, float);

#include "../RenderFramework/ShaderDataGen.h"