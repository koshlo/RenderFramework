
/* * * * * * * * * * * * * Author's note * * * * * * * * * * * *\
*   _       _   _       _   _       _   _       _     _ _ _ _   *
*  |_|     |_| |_|     |_| |_|_   _|_| |_|     |_|  _|_|_|_|_|  *
*  |_|_ _ _|_| |_|     |_| |_|_|_|_|_| |_|     |_| |_|_ _ _     *
*  |_|_|_|_|_| |_|     |_| |_| |_| |_| |_|     |_|   |_|_|_|_   *
*  |_|     |_| |_|_ _ _|_| |_|     |_| |_|_ _ _|_|  _ _ _ _|_|  *
*  |_|     |_|   |_|_|_|   |_|     |_|   |_|_|_|   |_|_|_|_|    *
*                                                               *
*                     http://www.humus.name                     *
*                                                                *
* This file is a part of the work done by Humus. You are free to   *
* use the code in any way you like, modified, unmodified or copied   *
* into your own work. However, I expect you to respect these points:  *
*  - If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  - For use in anything commercial, please request my approval.     *
*  - Share your work and ideas too as much as you can.             *
*                                                                *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _DIRECT3D10RENDERER_H_
#define _DIRECT3D10RENDERER_H_


#include "../Renderer.h"
#include <d3d11.h>

#define SAFE_RELEASE(p) { if (p){ p->Release(); p = NULL; } }

#define ROLLING_VB_SIZE (64 * 1024)

/*
#define VB_INVALID (-2)
*/

enum ShaderType
{
	Shader_VS,
	Shader_GS,
	Shader_PS,
	Shader_CS,

	Shader_Count
};

class Direct3D11Renderer : public GraphicsDevice
{
public:
	Direct3D11Renderer(ID3D11Device *d3ddev, ID3D11DeviceContext *ctx);
	~Direct3D11Renderer();

	void resetToDefaults();
	void reset(const uint flags = RESET_ALL);

//	bool resetDevice();

	TextureID addTexture(ID3D11Resource *resource, uint flags = 0);
	TextureID addTexture(Image &img, const SamplerStateID samplerState = SS_NONE, uint flags = 0);

	TextureID addRenderTarget(const int width, const int height, const int depth, const int mipMapCount, const int arraySize, const FORMAT format, const int msaaSamples = 1, const SamplerStateID samplerState = SS_NONE, uint flags = 0);
	TextureID addRenderDepth(const int width, const int height, const int arraySize, const FORMAT format, const int msaaSamples = 1, const SamplerStateID samplerState = SS_NONE, uint flags = 0);

	bool resizeRenderTarget(const TextureID renderTarget, const int width, const int height, const int depth, const int mipMapCount, const int arraySize);
	bool generateMipMaps(const TextureID renderTarget);

	void removeTexture(const TextureID texture);

    void getTextureSize(const TextureID texture, uint* width, uint* height) const;

	ShaderID addShader(const char *vsText, const char *gsText, const char *fsText, const int vsLine, const int gsLine, const int fsLine,
		const char *header = NULL, const char *extra = NULL, const char *fileName = NULL, const char **attributeNames = NULL, const int nAttributes = 0, const uint flags = 0);
    ShaderID addComputeShader(const char* src, const char** difenes, const int nDefines, const char* filename);
	VertexFormatID addVertexFormat(const FormatDesc *formatDesc, const uint nAttribs, const ShaderID shader = SHADER_NONE);
	VertexBufferID addVertexBuffer(const long size, const BufferAccess bufferAccess, const void *data = NULL);
	IndexBufferID addIndexBuffer(const uint nIndices, const uint indexSize, const BufferAccess bufferAccess, const void *data = NULL);
	StructuredBufferID addStructuredBuffer(const uint stride, const uint numElements, const bool addUAV);

	SamplerStateID addSamplerState(const Filter filter, const AddressMode s, const AddressMode t, const AddressMode r, const float lod = 0, const uint maxAniso = 16, const int compareFunc = 0, const float *border_color = NULL);
	BlendStateID addBlendState(const int srcFactorRGB, const int destFactorRGB, const int srcFactorAlpha, const int destFactorAlpha, const int blendModeRGB, const int blendModeAlpha, const int mask = ALL, const bool alphaToCoverage = false);
	DepthStateID addDepthState(const bool depthTest, const bool depthWrite, const int depthFunc, const bool stencilTest, const uint8 stencilReadMask, const uint8 stencilWriteMask,
		const int stencilFuncFront, const int stencilFuncBack, const int stencilFailFront, const int stencilFailBack,
		const int depthFailFront, const int depthFailBack, const int stencilPassFront, const int stencilPassBack);
	RasterizerStateID addRasterizerState(const int cullMode, const int fillMode = SOLID, const bool multiSample = true, const bool scissor = false, const float depthBias = 0.0f, const float slopeDepthBias = 0.0f);

	void setTexture(const char *textureName, const TextureID texture);
	void setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState);
	void setTextureSlice(const char *textureName, const TextureID texture, const int slice);
    void setUnorderedAccessTexture(const char *textureName, const TextureID texture, const int slice = NO_SLICE);
	void setStructBuffer(const char* bufferName, const StructuredBufferID buffer);
	void setReadWriteBuffer(const char* bufferName, const StructuredBufferID buffer);

	void applyTextures();

	void setSamplerState(const char *samplerName, const SamplerStateID samplerState);
	void applySamplerStates();

	void setShaderConstantRaw(const char *name, const void *data, const int size);
	void applyConstants();

//	void changeTexture(const uint imageUnit, const TextureID textureID);
	void changeRenderTargets(const TextureID *colorRTs, const uint nRenderTargets, const TextureID depthRT = TEXTURE_NONE, const int depthSlice = NO_SLICE, const int *slices = NULL);
	void changeToMainFramebuffer();
	void changeShader(const ShaderID shaderID);
	void changeVertexFormat(const VertexFormatID vertexFormatID);
	void changeVertexBuffer(const int stream, const VertexBufferID vertexBufferID, const intptr offset = 0);
	void changeIndexBuffer(const IndexBufferID indexBufferID);
	void changeCullFace(const int cullFace);

//	void changeSamplerState(const uint samplerUnit, const SamplerStateID samplerState);
	void changeBlendState(const BlendStateID blendState, const uint sampleMask = ~0);
	void changeDepthState(const DepthStateID depthState, const uint stencilRef = 0);
	void changeRasterizerState(const RasterizerStateID rasterizerState);


	void clear(const bool clearColor, const bool clearDepth, const bool clearStencil, const float *color, const float depth, const uint stencil);
	void clearRenderTarget(const TextureID renderTarget, const float4 &color, const int slice = NO_SLICE);
	void clearDepthTarget(const TextureID depthTarget, const float depth = 1.0f, const int slice = NO_SLICE);

	void dispatchCompute(uint threadGroupCountX, uint threadGroupCountY, uint threadGroupCountZ);

	void drawArrays(const Primitives primitives, const int firstVertex, const int nVertices);
	void drawElements(const Primitives primitives, const int firstIndex, const int nIndices, const int firstVertex, const int nVertices);
	void drawElementsInstanced(const Primitives primitives, const uint firstIndex, const uint nIndices, const uint firstVertex, const uint firstInstance, const uint nIstances);

	void setup2DMode(const float left, const float right, const float top, const float bottom);
	void drawPlain(const Primitives primitives, vec2 *vertices, const uint nVertices, const BlendStateID blendState, const DepthStateID depthState, const vec4 *color = NULL);
	void drawTextured(const Primitives primitives, TexVertex *vertices, const uint nVertices, const TextureID texture, const SamplerStateID samplerState, const BlendStateID blendState, const DepthStateID depthState, const vec4 *color = NULL);

	void setFrameBuffer(ID3D11RenderTargetView *colorRTV, ID3D11DepthStencilView *depthDSV)
	{
		backBufferRTV  = colorRTV;
		depthBufferDSV = depthDSV;
	}

	ID3D11Resource *getResource(const TextureID texture) const;

	void flush();
	void finish();

protected:
	void setShaderResourceViews(ShaderType shaderType, uint startSlot, uint numViews, ID3D11DeviceContext* context, ID3D11ShaderResourceView** views);
	void setSamplerState(ShaderType shaderType, uint startSlot, uint numSamplers, ID3D11DeviceContext* context, ID3D11SamplerState** samplerStates);

	ID3D11ShaderResourceView *createSRV(ID3D11Resource *resource, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, const int firstSlice = -1, const int sliceCount = -1);
	ID3D11RenderTargetView   *createRTV(ID3D11Resource *resource, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, const int firstSlice = -1, const int sliceCount = -1);
    ID3D11UnorderedAccessView *createUAV(ID3D11Resource *resource, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, const int firstSlice = -1, const int sliceCount = -1);
	ID3D11DepthStencilView   *createDSV(ID3D11Resource *resource, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, const int firstSlice = -1, const int sliceCount = -1);
    ID3D11UnorderedAccessView* createDefaultUAV(ID3D11Resource *resource);
	ID3D11Device *device;
	ID3D11DeviceContext *context;
	ID3D11RenderTargetView *backBufferRTV;
	ID3D11DepthStencilView *depthBufferDSV;

	TextureID currentTextures[Shader_Count][MAX_TEXTUREUNIT], selectedTextures[Shader_Count][MAX_TEXTUREUNIT];
	int currentTextureSlices[Shader_Count][MAX_TEXTUREUNIT], selectedTextureSlices[Shader_Count][MAX_TEXTUREUNIT];

	TextureID currentRwTexturesCS[MAX_UAV], selectedRwTexturesCS[MAX_UAV];
    int currentRwSlices[MAX_UAV], selectedRwSlices[MAX_UAV];

	SamplerStateID currentSamplerStates[Shader_Count][MAX_SAMPLERSTATE], selectedSamplerStates[Shader_Count][MAX_SAMPLERSTATE];

	StructuredBufferID currentStructBuffers[Shader_Count][MAX_STRUCT_BUFFER], selectedStructBuffers[Shader_Count][MAX_STRUCT_BUFFER];
	StructuredBufferID currentRwBuffers[MAX_STRUCT_BUFFER], selectedRwBuffers[MAX_STRUCT_BUFFER];

private:
	ubyte *mapRollingVB(const uint size);
	void unmapRollingVB(const uint size);
	uint copyToRollingVB(const void *src, const uint size);

	VertexBufferID rollingVB;
	int rollingVBOffset;

	ShaderID plainShader, texShader;
	VertexFormatID plainVF, texVF;

	float4 scaleBias2D;

	ID3D11Query *eventQuery;
};

#endif // _DIRECT3D10RENDERER_H_
