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

typedef TextureID TextureArrayID;

class IrradianceRenderer
{
public:
    IrradianceRenderer(GraphicsDevice* gfxDevice, RenderStateCache* stateCache, StateHelper* stateHelper);

    TextureArrayID BakeProbes(vec3* probePositions, uint probeCount, uint probeResolution, Scene& scene, uint numPasses);
    void DrawDebugSpheres(RenderQueue& renderQueue) const;
private:
    void GenerateDebugData(vec3* probePositions, uint probeCount);
    void BakePass(vec3* probePositions, uint probeCount, uint probeResolution, uint irradianceResolution, Scene& scene);

    typedef std::vector<ProbeDebugShaderData> ProbeDataArray;

    GraphicsDevice* _gfxDevice;
    RenderStateCache* _stateCache;
    StateHelper* _stateHelper;

    TextureArrayID _environmentMapsArray;
    TextureArrayID _irradianceMapsArray;
    TextureID _depthTarget;

    ShaderID _computeIrradiance;
    SamplerStateID _envMapSampler;

    ShaderID _debugSphereShader;
    RenderState _debugSphereState;
    ProbeDataArray _probeShaderData;
    SamplerStateID _sphereSampler;
    Model _sphereModel;
};