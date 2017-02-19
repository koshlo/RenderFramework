#include "DrawCall.h"
#include "Renderer.h"
#include "Util/Model.h"

void BatchDrawCall::Render(GraphicsDevice* gfxDevice, StateHelper* stateHelper, void* data)
{
	BatchDrawCall* dc = static_cast<BatchDrawCall*>(data);
    dc->geometry->drawBatch(gfxDevice, dc->batchNumber);
}

void IndexedInstancedDrawCall::Render(GraphicsDevice* gfxDevice, StateHelper* stateHelper, void* data)
{
	const IndexedInstancedDrawCall* dc = static_cast<const IndexedInstancedDrawCall*>(data);
	gfxDevice->changeVertexBuffer(0, dc->vertexBuffer);
	gfxDevice->changeIndexBuffer(dc->indexBuffer);
	gfxDevice->drawElementsInstanced(PRIM_TRIANGLES, dc->firstIndex, dc->indexCount, dc->firstVertex, dc->firstInstance, dc->instanceCount);
}
