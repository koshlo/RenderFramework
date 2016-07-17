#ifndef _SHADER_DATA_H_
#define _SHADER_DATA_H_

#ifdef __cplusplus

class StateHelper;

class ShaderData
{
public:
	virtual void Apply(StateHelper* stateHelper) const;
};

#endif // __cplusplus

#endif // _SHADER_DATA_H_