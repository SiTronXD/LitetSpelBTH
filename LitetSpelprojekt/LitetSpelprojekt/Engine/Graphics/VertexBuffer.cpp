#include "VertexBuffer.h"
#include "../Dev/Helpers.h"

VertexBuffer::VertexBuffer(Renderer& renderer)
	: Buffer(renderer),
	stride(sizeof(Vertex)),
	offset(0)
{
}

VertexBuffer::~VertexBuffer()
{
}

bool VertexBuffer::createBuffer(MeshData& meshData)
{
	return Buffer::createBuffer(
		D3D11_USAGE_IMMUTABLE,
		D3D11_BIND_VERTEX_BUFFER,
		sizeof(meshData.getVertices()[0]) * meshData.getVertices().size(),
		(void*) (VECTOR_ADDRESS(meshData.getVertices()))
	);
}
