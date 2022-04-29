#include "MeshLoader.h"
#include "../Dev/Log.h"

#include <DirectXMath.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

using namespace DirectX;

MeshLoader::MeshLoader()
{
}

MeshLoader::~MeshLoader()
{
}

MeshData MeshLoader::loadModel(const std::string& fileName, bool flipVerticalUV)
{
	MeshData createdMeshData;

	// Load
	const aiScene* scene = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene)
	{
		Log::error("Could not load mesh: " + std::string(aiGetErrorString()));
		return createdMeshData;
	}

	// Loop through each submesh
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* submesh = scene->mMeshes[i];

		// Loop through each vertex
		for (unsigned int j = 0; j < submesh->mNumVertices; ++j)
		{
			// Load data
			aiVector3D pos = submesh->mVertices[j];
			aiVector3D norm = submesh->mNormals[j];
			aiVector3D uv = submesh->mTextureCoords[0][j];

			if (flipVerticalUV)
				uv.y = 1.0f - uv.y;

			// Create vertex with data
			Vertex newVertex{};
			newVertex.pos = XMFLOAT3(pos.x, pos.y, pos.z);
			newVertex.normal = XMFLOAT3(norm.x, norm.y, norm.z);
			newVertex.uv = XMFLOAT2(uv.x, uv.y);

			createdMeshData.addVertex(newVertex);
		}

		// Loop through each index
		for (unsigned int j = 0; j < submesh->mNumFaces; ++j)
		{
			assert(submesh->mFaces[j].mNumIndices == 3u);

			createdMeshData.addIndex(submesh->mFaces[j].mIndices[0]);
			createdMeshData.addIndex(submesh->mFaces[j].mIndices[1]);
			createdMeshData.addIndex(submesh->mFaces[j].mIndices[2]);
		}
	}
		
	// Create one single submesh for entire model
	Submesh newSubmesh{};
	newSubmesh.startIndex = 0;
	newSubmesh.numIndices = createdMeshData.getIndices().size();
	createdMeshData.addSubmesh(newSubmesh);


	aiReleaseImport(scene);

	createdMeshData.invertFaces();
	createdMeshData.calculateNormals();

	return createdMeshData;
}
