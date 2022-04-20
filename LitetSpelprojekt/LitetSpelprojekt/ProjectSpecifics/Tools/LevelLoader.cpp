#include "LevelLoader.h"
#include "../../Engine/Dev/Log.h"

#include <DirectXMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

LevelLoader::LevelLoader(Resources& resources)
	: resources(resources),
	playerStartPos(0,0,0)
{
}

LevelLoader::~LevelLoader()
{
}

bool LevelLoader::load(const std::string& levelName)
{
	bool flipVerticalUV = true;

	// Load
	const aiScene* scene = aiImportFile(levelName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene)
	{
		Log::error("Could not load level: " + std::string(aiGetErrorString()));
		return false;
	}

	// Materials
	std::vector<std::string> diffuseTexturePaths(scene->mNumMaterials);
	for (int i = 0; i < scene->mNumMaterials; ++i)
	{
		// Get diffuse texture
		aiString path;
		if (scene->mMaterials[i]->GetTexture(
			aiTextureType_DIFFUSE,
			0,
			&path,
			NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			// Texture path
			diffuseTexturePaths[i] = std::string(path.C_Str());

			// Replace .tga with .png
			if (diffuseTexturePaths[i].find(".tga") != std::string::npos)
			{
				diffuseTexturePaths[i].erase(
					diffuseTexturePaths[i].size() - 3,
					3
				);
				diffuseTexturePaths[i] += "png";
			}

			// Add texture and material
			this->resources.addTexture(
				"Resources/Textures/" + diffuseTexturePaths[i],
				diffuseTexturePaths[i]
			);
			this->resources.addMaterial(
				diffuseTexturePaths[i],
				diffuseTexturePaths[i]
			);
		}
	}

	// Loop through each submesh
	unsigned int vertexOffset = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* submesh = scene->mMeshes[i];

		std::string submeshName = std::string(submesh->mName.C_Str());

		// Colliders
		if (submeshName.find("collider_") != std::string::npos)
		{

		}
		// Player start position
		else if (submeshName == "PlayerStartPos")
		{
			// Average position
			Vector3 sumPos = Vector3(0, 0, 0);
			for (unsigned int j = 0; j < submesh->mNumVertices; ++j)
			{
				sumPos += Vector3(
					submesh->mVertices[j].x,
					submesh->mVertices[j].y,
					submesh->mVertices[j].z
				);
			}
			sumPos /= (float) submesh->mNumVertices;

			// Apply
			this->playerStartPos = sumPos;
		}
		// Mesh
		else
		{
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

				this->meshData.addVertex(newVertex);
			}

			// Loop through each index
			unsigned int numIndices = 0;
			unsigned int indicesSize = this->meshData.getIndices().size();
			for (unsigned int j = 0; j < submesh->mNumFaces; ++j)
			{
				if (submesh->mFaces[j].mNumIndices != 3u)
					continue;

				this->meshData.addIndex(vertexOffset + submesh->mFaces[j].mIndices[0]);
				this->meshData.addIndex(vertexOffset + submesh->mFaces[j].mIndices[1]);
				this->meshData.addIndex(vertexOffset + submesh->mFaces[j].mIndices[2]);
			
				numIndices += 3;
			}

			Submesh newSubmesh{};
			newSubmesh.startIndex = indicesSize;
			newSubmesh.numIndices = numIndices;
			strcpy_s(newSubmesh.materialName, diffuseTexturePaths[submesh->mMaterialIndex].c_str());
			this->meshData.addSubmesh(newSubmesh);

			// Update offset for next submesh
			vertexOffset = this->meshData.getVertices().size();
		}
	}

	aiReleaseImport(scene);

	this->meshData.invertFaces();

	return true;
}
