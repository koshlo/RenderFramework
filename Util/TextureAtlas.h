#pragma once

#include <string>
#include "../Math/Vector.h"
#include "ArrayMap.h"

class GraphicsDevice;
class Model;
class ObjMaterialLib;

struct UVNode
{
    vec2 start;
    vec2 span;
};

class AtlasInfo
{
public:
    void AddNode(const std::string& nodeName, const UVNode& node) { _nodeMap.add(nodeName, node); }
    const UVNode& GetNode(const std::string& name) const { return _nodeMap[name]; }
private:
    ArrayMap<std::string, UVNode> _nodeMap;
};

void BuildAtlasInfo(const GraphicsDevice& gfxDevice, const Model& model, const ObjMaterialLib& materials, AtlasInfo* atlasInfo);