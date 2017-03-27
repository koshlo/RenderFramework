#ifdef __cplusplus

#define GENERATE_MEMBERS 1
#define GENERATE_ACCESSORS 2
#define GENERATE_APPLY 3

#include "ShaderData.h"

class SHADER_DATA_NAME : public ShaderData
{
public:
	virtual void Apply(StateHelper* stateHelper) const override
	{
#define GENERATION_PASS GENERATE_APPLY
#include "ShaderDataGen.def"
		SHADER_PARAM_LIST
	}

#undef GENERATION_PASS
#define GENERATION_PASS GENERATE_ACCESSORS
#include "ShaderDataGen.def"
	SHADER_PARAM_LIST

private:
#undef GENERATION_PASS
#define GENERATION_PASS GENERATE_MEMBERS
#include "ShaderDataGen.def"
	SHADER_PARAM_LIST
};

#undef SHADER_DATA_NAME
#undef SHADER_PARAM_LIST
#undef GENERATION_PASS

#else // __cplusplus

#define CONSTANT(Name, Type) Type Name
#define CONSTANT_ARRAY(Name, Type, Size) Type Name[Size]
#define TEXTURE_2D(Name, Type) Texture2D<Type> Name
#define TEXTURE_CUBE(Name, Type) TextureCube<Type> Name
#define TEXTURE_3D(Name, Type) Texture3D<Type> Name
#define SAMPLER_STATE(Name) SamplerState Name
#define UAV_TEXTURE_2D(Name, Type) RWTexture2D<Type> Name
#define UAV_TEXTURE_2D_SLICE(Name, Type) UAV_TEXTURE_2D(Name, Type)
#define UAV_TEXTURE_2D_ARRAY(Name, Type) RWTexture2DArray<Type> Name
#define BUFFER(Name, Type) StructuredBuffer<Type> Name
#define UAV_BUFFER(Name, Type) RWStructuredBuffer<Type> Name

SHADER_PARAM_LIST

#undef SHADER_DATA_NAME
#undef SHADER_PARAM_LIST

#endif // __cplusplus