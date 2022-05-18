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

	MeshData skeletonMeshData;

	float animationTimer;

	bool meshDataHasAnimations;

	template <typename T>
	void calcAlpha(
		std::vector<std::pair<double, T>> stamps,
		unsigned int& outputLowestIndex,
		unsigned int& outputHighestIndex,
		float& outputAlpha
	);
	void getLerpValues(
		std::vector<std::pair<double, DirectX::XMFLOAT3>> stamps,
		DirectX::XMVECTOR& output
	);
	void getSlerpValues(
		std::vector<std::pair<double, DirectX::XMFLOAT4>> stamps,
		DirectX::XMVECTOR& output
	);
	void getInterpolatedModelMat(BoneTransforms& boneTransforms, DirectX::XMMATRIX& output);
	void getTransformations(DirectX::XMFLOAT4X4*& mats);

public:
	Mesh(Renderer& renderer, MeshData&& meshData);
	virtual ~Mesh();

	void update();

	inline StructuredBuffer& getBoneTransformationsBuffer() { return this->boneTransformationsBuffer; }

	inline VertexBuffer& getVertexBuffer() { return this->vertexBuffer; }
	inline IndexBuffer& getIndexBuffer() { return this->indexBuffer; }
	inline std::vector<Submesh>& getSubmeshes() { return this->submeshes; }
	inline const Submesh& getEntireSubmesh() const { return this->entireMesh; }

	inline const bool& hasAnimations() const { return this->meshDataHasAnimations; }
};