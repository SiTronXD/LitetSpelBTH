#include "Mesh.h"
#include "../Time.h"

using namespace DirectX;

template <typename T>
void Mesh::calcAlpha(
	std::vector<std::pair<double, T>> stamps,
	unsigned int& outputLowestIndex,
	unsigned int& outputHighestIndex,
	float& outputAlpha)
{
	// Find lowest index of correct time interval
	outputLowestIndex = -1;
	for (unsigned int i = 0; i < stamps.size(); ++i)
	{
		if (this->animationTimer >= stamps[i].first)
			outputLowestIndex = i;
	}
	// Loop animation
	if (outputLowestIndex >= stamps.size() - 1)
	{
		// Subtract end pose time
		this->animationTimer -=
			stamps[stamps.size() - 1].first;

		// Start at the beginning
		outputLowestIndex = 0;
	}
	outputHighestIndex = outputLowestIndex + 1;

	// Alpha
	outputAlpha = (float)
		((this->animationTimer - stamps[outputLowestIndex].first) /
			(stamps[outputHighestIndex].first - stamps[outputLowestIndex].first));
}

void Mesh::getLerpValues(std::vector<std::pair<double, DirectX::XMFLOAT3>> stamps, DirectX::XMVECTOR& output)
{
	// 1 single stamp
	if (stamps.size() <= 1)
	{
		output = XMVectorSet(
			stamps[0].second.x,
			stamps[0].second.y,
			stamps[0].second.z,
			1.0f
		);

		return;
	}

	// Find lowest index of correct time interval
	unsigned int lowestIndex = -1;
	unsigned int highestIndex = -1;
	float alpha = -1.0f;
	this->calcAlpha(stamps, lowestIndex, highestIndex, alpha);

	// c = a0 * (1 - alpha) + a1 * (alpha)
	output = XMVectorSet(
		stamps[lowestIndex].second.x,
		stamps[lowestIndex].second.y,
		stamps[lowestIndex].second.z,
		1.0f
	) * (1.0f - alpha) +
		XMVectorSet(
			stamps[highestIndex].second.x,
			stamps[highestIndex].second.y,
			stamps[highestIndex].second.z,
			1.0f
		) * alpha;
}

void Mesh::getSlerpValues(std::vector<std::pair<double, DirectX::XMFLOAT4>> stamps, DirectX::XMVECTOR& output)
{
	// 1 single stamp
	if (stamps.size() <= 1)
	{
		output = XMVectorSet(
			stamps[0].second.x,
			stamps[0].second.y,
			stamps[0].second.z,
			stamps[0].second.w
		);

		return;
	}

	// Find lowest index of correct time interval
	unsigned int lowestIndex = -1;
	unsigned int highestIndex = -1;
	float alpha = -1.0f;
	this->calcAlpha(stamps, lowestIndex, highestIndex, alpha);

	output = XMQuaternionSlerp(
		XMVectorSet(
			stamps[lowestIndex].second.x,
			stamps[lowestIndex].second.y,
			stamps[lowestIndex].second.z,
			stamps[lowestIndex].second.w
		),
		XMVectorSet(
			stamps[highestIndex].second.x,
			stamps[highestIndex].second.y,
			stamps[highestIndex].second.z,
			stamps[highestIndex].second.w
		),
		alpha
	);
}

void Mesh::getInterpolatedModelMat(BoneTransforms& boneTransforms, XMMATRIX& output)
{
	// Interpolate
	XMVECTOR interpolatedScaling = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR interpolatedRotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR interpolatedPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	this->getLerpValues(boneTransforms.scaleStamps, interpolatedScaling);
	this->getSlerpValues(boneTransforms.rotationStamps, interpolatedRotation);
	this->getLerpValues(boneTransforms.positionStamps, interpolatedPosition);

	// Composite
	output *= XMMatrixScalingFromVector(interpolatedScaling);
	output *= XMMatrixRotationQuaternion(interpolatedRotation);
	output *= XMMatrixTranslationFromVector(interpolatedPosition);
}

void Mesh::getTransformations(XMFLOAT4X4*& mats)
{
	for (unsigned int i = 0; i < this->skeletonMeshData.getSkeleton().size(); ++i)
	{
		Bone& bone = this->skeletonMeshData.getSkeleton()[i];
		BoneTransforms& boneTransforms = bone.boneTransforms;

		XMMATRIX animLocalMat = XMMatrixIdentity();
		this->getInterpolatedModelMat(boneTransforms, animLocalMat);
		XMMATRIX finalTransformation = XMLoadFloat4x4(&bone.bindModelInverse);
		XMMATRIX traversedModelMat = animLocalMat;

		// Parent model matrix
		if (bone.parentIndex >= 0)
		{
			traversedModelMat *=
				XMLoadFloat4x4(&skeletonMeshData.getSkeleton()[bone.parentIndex].modelTransform);
		}
		finalTransformation *= traversedModelMat;
		XMStoreFloat4x4(&bone.modelTransform, traversedModelMat);

		XMStoreFloat4x4(&mats[i], XMMatrixTranspose(finalTransformation));
	}
}

Mesh::Mesh(Renderer& renderer, MeshData&& meshData)
	: vertexBuffer(renderer),
	indexBuffer(renderer),
	submeshes(meshData.getSubmeshes()),
	meshDataHasAnimations(meshData.hasAnimations()),
	boneTransformationsBuffer(renderer, "boneTransformationsBuffer"),
	entireMesh{ "", 0, static_cast<unsigned int>(meshData.getIndices().size()) },
	animationTimer(0.0f)
{
	this->vertexBuffer.createBuffer(meshData);
	this->indexBuffer.createBuffer(meshData);

	// Bone transformations buffer
	if (meshData.hasAnimations())
	{
		// Transformation matrices, init to identity
		this->boneTransformationMats = new XMFLOAT4X4[
			meshData.getSkeleton().size()
		];
		for (unsigned int i = 0; i < meshData.getSkeleton().size(); ++i)
		{
			XMStoreFloat4x4(
				&boneTransformationMats[i],
				XMMatrixTranspose(XMMatrixIdentity())
			);
		}

		// Create buffer
		this->boneTransformationsBuffer.createBuffer(
			sizeof(XMFLOAT4X4),
			(UINT) meshData.getSkeleton().size(),
			this->boneTransformationMats,
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_SHADER_RESOURCE,
			true
		);

		// Get important mesh data
		this->skeletonMeshData = meshData;
		this->skeletonMeshData.getAnimVertices().clear();
		this->skeletonMeshData.getIndices().clear();
	}
}

Mesh::~Mesh()
{
	delete[] boneTransformationMats;
}

void Mesh::update(float animationTimer)
{
	// Only update animated meshes
	if (!this->hasAnimations())
		return;

	this->animationTimer = animationTimer;

	this->getTransformations(this->boneTransformationMats);
	this->boneTransformationsBuffer.cpuUpdateBuffer(this->boneTransformationMats);
}
