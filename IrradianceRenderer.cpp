#include "IrradianceRenderer.h"
#include "Math/Vector.h"

TextureID* IraddianceRenderer::BakeProbes(vec3* positions, uint count)
{
    _environmentMaps.reserve(count);
    _irradianceMaps.reserve(count);
    for (uint i = 0; i < count; ++i)
    {
        for (uint face = 0; face < 6; ++face)
        {
            mat4 cubeFaceView = cubeViewMatrix(face);
            cubeFaceView.translate(-positions[i]);
            mat4 faceViewProj = cubeProjectionMatrixD3D(0.001f, 2000.0f) * cubeFaceView;

        }
    }
    return nullptr;
}
