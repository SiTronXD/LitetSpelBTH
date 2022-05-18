#pragma once

#include <map>
#include <string>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include "MeshData.h"

class MeshLoader
{
private:
	static void traverseSkeleton(
		std::map<std::string, DirectX::XMFLOAT4X4>& bindInverseMatrices,
		MeshData& meshData,
		aiNode* node, unsigned int& globalBoneIndex,
		int lastParentIndex,
		const std::string& space
	);

	static void loadPoses(
		std::map<std::string, DirectX::XMFLOAT4X4>& bindInverseMatrices,
		MeshData& meshData,
		const aiScene* scene
	);

public:
	MeshLoader();
	virtual ~MeshLoader();

	static MeshData loadModel(
		const std::string& fileName, bool flipVerticalUV = true
	);

	static MeshData loadAnimatedModel(
		const std::string& fileName, bool flipVerticalUV = true
	);
};