#include "Mesh.h"

Mesh::Mesh(Renderer& renderer, MeshData&& meshData)
	: vertexBuffer(renderer),
	indexBuffer(renderer)
{
	this->vertexBuffer.createBuffer(meshData);
	this->indexBuffer.createBuffer(meshData);
}

Mesh::~Mesh()
{
}