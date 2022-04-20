#pragma once

#include <string>
#include <DirectXMath.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include "../../Engine/Graphics/MeshData.h"
#include "../../Engine/Resources.h"

class LevelLoader
{
private:
	Resources& resources;

	MeshData meshData;

	DirectX::SimpleMath::Vector3 playerStartPos;

	void traverseFile(aiNode* node);

public:
	LevelLoader(Resources& resources);
	virtual ~LevelLoader();

	bool load(const std::string& levelName);

	inline MeshData& getMeshData() { return this->meshData; }

	inline DirectX::SimpleMath::Vector3& getPlayerStartPos() { return this->playerStartPos; }
};
