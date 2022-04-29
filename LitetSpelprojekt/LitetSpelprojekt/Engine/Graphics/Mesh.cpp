#include "Mesh.h"

Mesh::Mesh(Renderer& renderer, MeshData&& meshData)
	: vertexBuffer(renderer),
	indexBuffer(renderer),
	submeshes(meshData.getSubmeshes()),
	entireMesh{ "", 0, static_cast<unsigned int>(meshData.getIndices().size()) }
{
	this->vertexBuffer.createBuffer(meshData);
	this->indexBuffer.createBuffer(meshData);
}

Mesh::~Mesh()
{
}