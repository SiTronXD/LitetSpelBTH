#include "Mesh.h"

Mesh::Mesh(Renderer& renderer)
	: vertexBuffer(renderer),
	indexBuffer(renderer)
{
	MeshData meshData(DefaultMesh::CUBE);
	this->vertexBuffer.createBuffer(meshData);
	this->indexBuffer.createBuffer(meshData);
}

Mesh::~Mesh()
{
}