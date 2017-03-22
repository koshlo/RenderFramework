#pragma once

#include <vector>

#include "RenderDefs.h"
#include "Platform.h"

class IraddianceRenderer
{
public:
    TextureID* BakeProbes(vec3* positions, uint count);
private:
    typedef std::vector<TextureID> TextureArray;

    TextureArray _environmentMaps;
    TextureArray _irradianceMaps;
};