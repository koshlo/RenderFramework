#include "ParsingHelpers.h"
#include "Tokenizer.h"

int readSignedInt(Tokenizer &tok) {
	char *str = tok.next();
	if (str[0] == '-') {
		str = tok.next();
		return -atoi(str);
	}
	else {
		if (str[0] == '+') {
			str = tok.next();
		}
		return atoi(str);
	}
}

float readFloat(Tokenizer &tok) {
	char *str = tok.next();
	if (str[0] == '-') {
		str = tok.next();
		return -(float)atof(str);
	}
	else {
		if (str[0] == '+') {
			str = tok.next();
		}
		return (float)atof(str);
	}
}

float3 readVector3(Tokenizer& tok)
{
	float x = readFloat(tok);
	float y = readFloat(tok);
	float z = readFloat(tok);

	return float3(x, y, z);
}

float4 readVector4(Tokenizer& tok)
{
	float3 xyz = readVector3(tok);
	float w = readFloat(tok);
	return float4(xyz, w);
}