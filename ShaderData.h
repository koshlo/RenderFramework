#ifndef _SHADER_DATA_H_
#define _SHADER_DATA_H_

#ifdef __cplusplus

#include "StateHelper.h"

class ShaderData
{
public:
	virtual void Apply(StateHelper* stateHelper) const = 0;
};

#endif // __cplusplus

#endif // _SHADER_DATA_H_