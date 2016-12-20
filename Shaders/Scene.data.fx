#define SHADER_DATA_NAME SceneShaderData

#define SHADER_PARAM_LIST \
CONSTANT(ViewProjection, float4x4); \
CONSTANT(InvViewProjection, float4x4); \
CONSTANT(Viewport, float2); \
CONSTANT(SunDirection, float3); \
CONSTANT(SunIntensity, float3); \
CONSTANT(Ambient, float3);

#include "../ShaderDataGen.h"