#pragma once

#include <string>
#include <DirectXMath.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include "../../Engine/Graphics/MeshData.h"
#include "../../Engine/Resources.h"
#include <assimp/texture.h>

struct LevelColliderSphere
{
	DirectX::XMFLOAT3 pos;
	float radius;
};

struct LevelColliderBox
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 extents;
};

struct LevelColliderOrientedBox
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 extents;
	DirectX::XMFLOAT4 orientation;
};

struct KeyInfo
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 color;
};

struct PortalInfo
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 scale;
};

struct SpikeInfo
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector4 rotation;
};

class LevelLoader
{
private:
	const unsigned int NUM_KEYS = 4;

	Resources& resources;

	std::vector<MeshData*> allMeshes;

	MeshData megaMesh;

	DirectX::SimpleMath::Vector3 playerStartPos;

	PortalInfo portal;

	std::vector<LevelColliderSphere> sphereColliders;
	std::vector<LevelColliderBox> boxColliders;
	std::vector<LevelColliderOrientedBox> orientedBoxColliders;
	std::vector<KeyInfo> keys;
	std::vector<SpikeInfo> spikes;

	void switchFloats(DirectX::SimpleMath::Vector3& vec);
	void switchFloats(DirectX::XMFLOAT3& vec);

	void traverseStructure(
		aiNode* node,
		const DirectX::SimpleMath::Matrix& parentTransform
	);
	void addMeshToMegaMesh(
		MeshData meshData, 
		const DirectX::SimpleMath::Matrix transformation
	);

	DirectX::SimpleMath::Vector3 getAveragePosition(aiMesh* submesh);

public:
	LevelLoader(Resources& resources);
	virtual ~LevelLoader();

	bool load(const std::string& levelName);

	inline MeshData& getMeshData() { return this->megaMesh; }

	inline DirectX::SimpleMath::Vector3& getPlayerStartPos() { return this->playerStartPos; }
	inline PortalInfo& getPortal() { return this->portal; }
	inline std::vector<LevelColliderSphere>& getSphereColliders() { return this->sphereColliders; }
	inline std::vector<LevelColliderBox>& getBoxColliders() { return this->boxColliders; }
	inline std::vector<LevelColliderOrientedBox>& getOrientedBoxColliders() { return this->orientedBoxColliders; }
	inline std::vector<KeyInfo>& getKeys() { return this->keys; }
	inline std::vector<SpikeInfo>& getSpikes() { return this->spikes; }
};
