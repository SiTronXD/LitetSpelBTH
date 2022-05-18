#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "StructuredBuffer.h"

class Mesh
{
private:
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	std::vector<Submesh> submeshes;
	Submesh entireMesh;

	StructuredBuffer boneTransformationsBuffer;

	bool meshDataHasAnimations;

public:
	Mesh(Renderer& renderer, MeshData&& meshData);
	virtual ~Mesh();

	inline StructuredBuffer& getBoneTransformationsBuffer() { return this->boneTransformationsBuffer; }

	inline VertexBuffer& getVertexBuffer() { return this->vertexBuffer; }
	inline IndexBuffer& getIndexBuffer() { return this->indexBuffer; }
	inline std::vector<Submesh>& getSubmeshes() { return this->submeshes; }
	inline const Submesh& getEntireSubmesh() const { return this->entireMesh; }

	inline const bool& hasAnimations() const { return this->meshDataHasAnimations; }
};