#include "IrradianceRenderer.h"
#include "Math/Vector.h"
#include "Renderer.h"
#include "RenderQueue.h"
#include "Util/SceneObject.h"
#include "Util/Helpers.h"
#include "Shaders/View.data.fx"
#include "Shaders/ConvolveEnvMap.data.fx"

IrradianceRenderer::IrradianceRenderer(GraphicsDevice* gfxDevice, RenderStateCache* stateCache, StateHelper* stateHelper)
    : _gfxDevice(gfxDevice)
    , _stateCache(stateCache)
    , _debugSphereShader(SHADER_NONE)
    , _stateHelper(stateHelper)
{
    _computeIrradiance = _gfxDevice->addComputeShader("../RenderFramework/Shaders/ConvolveEnvMap.fx");
    _envMapSampler = _stateCache->GetSamplerState(SamplerStateDesc{ LINEAR, CLAMP, CLAMP, CLAMP });
}

TextureID* IrradianceRenderer::BakeProbes(vec3* probePositions, uint probeCount, uint probeResolution, const Scene& scene)
{
    uint irrRes = probeResolution / 2;

    if (_debugSphereShader == SHADER_NONE)
    {
        _environmentMapsArray = _gfxDevice->addRenderTarget(probeResolution, probeResolution, 1, 1, probeCount, FORMAT_RGBA16F, 1, SS_NONE, CUBEMAP | RENDER_SLICES);
        _irradianceMapsArray = _gfxDevice->addRenderTarget(irrRes, irrRes, 1, 1, probeCount, FORMAT_RGBA16F, 1, SS_NONE, CUBEMAP | ADD_UAV);
    }

    TextureID depthTarget = _gfxDevice->addRenderDepth(probeResolution, probeResolution, 16);
    for (uint i = 0; i < probeCount; ++i)
    {
        ViewShaderData viewData;
        viewData.SetEyePos(float4(probePositions[i], 1.0f));
        float fResolution = static_cast<float>(probeResolution);
        viewData.SetViewport(float2(fResolution, fResolution));
        for (uint face = 0; face < 6; ++face)
        {
            RenderQueue renderQueue(_gfxDevice, &_environmentMapsArray, 1, depthTarget, i * 6 + face);
            
            mat4 cubeFaceView = cubeViewMatrix(face);
            cubeFaceView.translate(-probePositions[i]);
            mat4 faceViewProj = cubeProjectionMatrixD3D(0.0001f, 2000.0f) * cubeFaceView;
            viewData.SetViewProjection(faceViewProj);
            
            renderQueue.AddShaderData(&viewData);
            renderQueue.AddShaderData(scene.shadowShaderData);
            renderQueue.AddShaderData(scene.lightShaderData);
            renderQueue.AddShaderData(scene.expWarpingData);

            for (uint iObj = 0; iObj < scene.numObjects; ++iObj)
            {
                scene.objects[iObj].Draw(renderQueue, iObj);
            }

            renderQueue.SubmitAll(_gfxDevice, _stateHelper);
        }

        ConvolveEnvMapShaderData irradianceShaderData;
        irradianceShaderData.SetEnvironmentMapArray(_environmentMapsArray);
        irradianceShaderData.SetEnvMapSampler(_envMapSampler);
        irradianceShaderData.SetIrradianceCubeMapUAV(_irradianceMapsArray);
        irradianceShaderData.SetResolution(float2(irrRes - 1.0f, irrRes - 1.0f));
        irradianceShaderData.SetFrameRandom(float2(1, 1));
        irradianceShaderData.SetProbeIndex(i);

        DispatchGroup group{ irrRes / NUM_THREADS, irrRes / NUM_THREADS, 1 };
        const ShaderData* shaderData[] = { &irradianceShaderData };
        for (uint face = 0; face < 6; ++face)
        {
            mat4 faceView = cubeViewMatrix(face);
            irradianceShaderData.SetRight(faceView.rows[0].xyz());
            irradianceShaderData.SetUp(faceView.rows[1].xyz());
            irradianceShaderData.SetNormal(faceView.rows[2].xyz());
            irradianceShaderData.SetFace(face);
            RenderQueue::DispatchCompute(_stateHelper, group, _computeIrradiance, shaderData, array_size(shaderData));
        }
    }

    GenerateDebugData(probePositions, probeCount);

    return nullptr;
}

void IrradianceRenderer::DrawDebugSpheres(RenderQueue& renderQueue) const
{
    for (uint i = 0; i < _probeShaderData.size(); ++i)
    {
        const ProbeDebugShaderData& currentData = _probeShaderData[i];
        DrawCallState dcState{ &_debugSphereState, &currentData };
        BatchDrawCall& batchCall = renderQueue.AddRenderCommand<BatchDrawCall>(i, dcState);
        batchCall.geometry = &_sphereModel;
        batchCall.batchNumber = 0;
    }
}

void IrradianceRenderer::GenerateDebugData(vec3* probePositions, uint probeCount)
{
    if (_debugSphereShader != SHADER_NONE)
        return;

    _sphereModel.createSphere(5);
    _sphereModel.computeNormals();

    _debugSphereShader = _gfxDevice->addShader("../RenderFramework/Shaders/ProbeDebug.fx");
    _sphereModel.makeDrawable(_gfxDevice, true, _debugSphereShader);

    RenderStateDesc desc;
    desc.shader = _debugSphereShader;
    desc.depthWrite = true;
    desc.depthTest = true;
    desc.depthOp = GEQUAL;
    desc.cullState = CULL_NONE;
    _debugSphereState = _stateCache->GetRenderState(desc);

    _sphereSampler = _stateCache->GetSamplerState(SamplerStateDesc{ LINEAR, CLAMP, CLAMP, CLAMP });
    _probeShaderData.resize(probeCount);
    for (uint i = 0; i < probeCount; ++i)
    {
        ProbeDebugShaderData& currentData = _probeShaderData[i];
        static const float scale = 20.0f;
        currentData.SetTranslationScale(float4(probePositions[i], scale));
        currentData.SetCubeMapArray(_irradianceMapsArray);
        currentData.SetCubeMapSampler(_sphereSampler);
        currentData.SetProbeIndex(i);
    }
}
