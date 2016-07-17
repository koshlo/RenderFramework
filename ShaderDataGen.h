#ifdef __cplusplus

#define GENERATE_MEMBERS 1
#define GENERATE_ACCESSORS 2
#define GENERATE_APPLY 3

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

#else // __cplusplus

#define CONSTANT_2F(Name) float2 _##Name
#define CONSTANT_4F(Name) float4 _##Name
#define CONSTANT_MATRIX_4F(Name) float4x4 _##Name
#define TEXTURE_2D(Name, Type) Texture2D<Type> _##Name
#define TEXTURE_3D(Name, Type) Texture3D<Type> _##Name
#define SAMPLER_STATE(Name) SamplerStateID _##Name
#define UAV_TEXTURE2D(Name) RWTexture _##Name

#endif // __cplusplus