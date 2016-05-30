#ifndef _PARSING_HELPERS_H_
#define _PARSING_HELPERS_H_

#include "../Math/Vector.h"

class Tokenizer;

int readSignedInt(Tokenizer& tok);
float readFloat(Tokenizer& tok);
float4 readVector4(Tokenizer& tok);
float3 readVector3(Tokenizer& tok);

#endif // _PARSING_HELPERS_H_