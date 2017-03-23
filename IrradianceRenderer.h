#pragma once

#include <vector>

#include "RenderDefs.h"
#include "Platform.h"
#include "Shaders/ProbeDebug.data.fx"
#include "Util/Model.h"
#include "RenderState.h"

class GraphicsDevice;
class RenderStateCache;
class RenderQueue;
struct Scene;

class IrradianceRenderer
{
public:
    IrradianceRenderer(GraphicsDevice* gfxDevice, RenderStateCache* stateCache, StateHelper* stateHelper);

    TextureID* BakeProbes(vec3* probePositions, uint probeCount, uint probeResolution, const Scene& scene);
    void DrawDebugSpheres(RenderQueue& renderQueue) const;
private:
    void GenerateDebugData(vec3* probePositions, uint probeCount);

    typedef std::vector<TextureID> TextureArray;
    typedef std::vector<ProbeDebugShaderData> ProbeDataArray;

    GraphicsDevice* _gfxDevice;
    RenderStateCache* _stateCache;
    StateHelper* _stateHelper;

    TextureArray _environmentMaps;
    TextureArray _irradianceMaps;

    ShaderID _debugSphereShader;
    RenderState _debugSphereState;
    ProbeDataArray _probeShaderData;
    SamplerStateID _sphereSampler;
    Model _sphereModel;
};