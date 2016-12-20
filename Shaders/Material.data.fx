#define SHADER_DATA_NAME MaterialShaderData

#define SHADER_PARAM_LIST \
TEXTURE_2D(AlbedoMap, float4); \
TEXTURE_2D(NormalMap, float4); \
TEXTURE_2D(RoughnessMap, float4); \
TEXTURE_2D(MetallicMap, float4);

#include "../ShaderDataGen.h"