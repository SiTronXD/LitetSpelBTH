#include "VertexBuffer.h"
#include "../Dev/Helpers.h"

VertexBuffer::VertexBuffer(Renderer& renderer)
	: Buffer(renderer, "Vertex Buffer"),
	stride(-1),
	offset(0)
{
}

VertexBuffer::~VertexBuffer()
{
}

bool VertexBuffer::createBuffer(MeshData& meshData)
{
	if (!meshData.hasAnimations())
	{
		this->stride = sizeof(Vertex);

		return Buffer::createBuffer(
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_VERTEX_BUFFER,
			sizeof(meshData.getVertices()[0]) * meshData.getVertices().size(),
			(void*)(VECTOR_ADDRESS(meshData.getVertices()))
		);
	}
	else
	{
		this->stride = sizeof(AnimVertex);

		return Buffer::createBuffer(
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_VERTEX_BUFFER,
			sizeof(meshData.getAnimVertices()[0]) * meshData.getAnimVertices().size(),
			(void*)(VECTOR_ADDRESS(meshData.getAnimVertices()))
		);
	}
}
