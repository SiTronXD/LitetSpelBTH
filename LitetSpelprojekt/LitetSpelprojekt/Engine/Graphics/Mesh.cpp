#include "Mesh.h"

using namespace DirectX;

Mesh::Mesh(Renderer& renderer, MeshData&& meshData)
	: vertexBuffer(renderer),
	indexBuffer(renderer),
	submeshes(meshData.getSubmeshes()),
	meshDataHasAnimations(meshData.hasAnimations()),
	boneTransformationsBuffer(renderer, "boneTransformationsBuffer"),
	entireMesh{ "", 0, static_cast<unsigned int>(meshData.getIndices().size()) }
{
	this->vertexBuffer.createBuffer(meshData);
	this->indexBuffer.createBuffer(meshData);

	// Bone transformations buffer
	if (meshData.hasAnimations())
	{
		// Fill array with identity matrices
		XMFLOAT4X4* initialTransformationData = new XMFLOAT4X4[
			meshData.getSkeleton().size()
		];
		for (unsigned int i = 0; i < meshData.getSkeleton().size(); ++i)
		{
			XMStoreFloat4x4(
				&initialTransformationData[i],
				XMMatrixTranspose(XMMatrixIdentity())
			);
		}

		// Create buffer
		this->boneTransformationsBuffer.createBuffer(
			sizeof(XMFLOAT4X4),
			(UINT) meshData.getSkeleton().size(),
			initialTransformationData
		);

		// Deallocate initial data
		delete[] initialTransformationData;
	}
}

Mesh::~Mesh()
{
}