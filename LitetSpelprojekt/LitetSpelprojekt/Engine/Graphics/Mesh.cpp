#include "Mesh.h"

Mesh::Mesh(Renderer& renderer, MeshData&& meshData)
	: vertexBuffer(renderer),
	indexBuffer(renderer),
	submeshes(meshData.getSubmeshes())
{
	this->vertexBuffer.createBuffer(meshData);
	this->indexBuffer.createBuffer(meshData);
}

Mesh::~Mesh()
{
}