#define SHADER_DATA_NAME ViewShaderData

#define SHADER_PARAM_LIST \
CONSTANT(ViewProjection, float4x4); \
CONSTANT(InvViewProjection, float4x4); \
CONSTANT(Viewport, float2);

#include "../ShaderDataGen.h"