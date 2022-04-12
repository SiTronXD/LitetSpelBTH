#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
private:
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

public:
	Mesh(Renderer& renderer, MeshData&& meshData);
	virtual ~Mesh();

	inline VertexBuffer& getVertexBuffer() { return this->vertexBuffer; }
	inline IndexBuffer& getIndexBuffer() { return this->indexBuffer; }
};