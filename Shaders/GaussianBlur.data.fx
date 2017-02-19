#define SHADER_DATA_NAME BlurShaderData

#define MAX_BLUR_SIZE 32

#define SHADER_PARAM_LIST \
TEXTURE_2D(SourceTexture, float2); \
UAV_TEXTURE_2D(DestTexture, float2); \
CONSTANT(SampleParams, float4); \
CONSTANT(BlurHalfSize, uint); \
CONSTANT_ARRAY(Weights, float4, MAX_BLUR_SIZE / 4);

#include "../RenderFramework/ShaderDataGen.h"