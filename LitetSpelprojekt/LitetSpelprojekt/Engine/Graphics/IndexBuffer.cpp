#include "IndexBuffer.h"
#include "../Dev/Helpers.h"

IndexBuffer::IndexBuffer(Renderer& renderer)
	: Buffer(renderer, "Index Buffer"),
	indexCount(-1)
{
}

IndexBuffer::~IndexBuffer()
{
}

bool IndexBuffer::createBuffer(MeshData& meshData)
{
	this->indexCount = meshData.getIndices().size();

	return Buffer::createBuffer(
		D3D11_USAGE_IMMUTABLE,
		D3D11_BIND_INDEX_BUFFER,
		sizeof(meshData.getIndices()[0]) * this->indexCount,
		(void*) (VECTOR_ADDRESS(meshData.getIndices()))
	);
}
