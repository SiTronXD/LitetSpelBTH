#include "MeshLoader.h"
#include "../Dev/Log.h"
#include "../SMath.h"

#include <DirectXMath.h>

using namespace DirectX;

#define PRINT_SKELETAL_INFO

void MeshLoader::traverseSkeleton(
	std::map<std::string, XMFLOAT4X4>& bindInverseMatrices,
	MeshData& meshData,
	aiNode* node,
	unsigned int& globalBoneIndex,
	int lastParentIndex,
	const std::string& space)
{
	std::string nodeName = std::string(node->mName.C_Str());
	bool isNodeBone = bindInverseMatrices.count(nodeName);

	// This node is a bone, apply bind inverse matrix and swap in hierarchy
	unsigned int thisBoneIndex = -1;
	if (isNodeBone)
	{
		Bone* currentBone = nullptr;
		unsigned int currentBoneIndex = meshData.getBoneIndex(nodeName, currentBone);

#ifdef PRINT_SKELETAL_INFO
		Log::write(space + nodeName + "  " + std::to_string(globalBoneIndex));
#endif

		currentBone->bindModelInverse = bindInverseMatrices[nodeName];

		// Make sure this index is always lower than child indices
		meshData.swapBones(*currentBone, globalBoneIndex, currentBoneIndex);
		thisBoneIndex = globalBoneIndex;
		globalBoneIndex++;
	}
#ifdef PRINT_SKELETAL_INFO
	else
		Log::write(space + nodeName + "--- " + std::to_string(lastParentIndex));
#endif

	// Continue to traverse through children
	std::string childNodeName;
	unsigned int nextParentIndex = isNodeBone ? thisBoneIndex : lastParentIndex;
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		childNodeName = std::string(node->mChildren[i]->mName.C_Str());

		// Set parent index for child
		if (bindInverseMatrices.count(childNodeName))
		{
			Bone* childBone = nullptr;
			meshData.getBoneIndex(childNodeName, childBone);
			childBone->parentIndex = nextParentIndex;
		}

		// Traverse
		traverseSkeleton(
			bindInverseMatrices, meshData, node->mChildren[i], globalBoneIndex, nextParentIndex, space + " "
		);
	}
}

void MeshLoader::loadPoses(
	std::map<std::string, XMFLOAT4X4>& bindInverseMatrices,
	MeshData& meshData,
	const aiScene* scene
)
{
	// No animations
	if (!scene->HasAnimations())
		return;

	aiAnimation* anim = scene->mAnimations[0];

	// Go through each channel to find the bones
	std::string channelNodeName;
	for (unsigned int i = 0; i < anim->mNumChannels; ++i)
	{
		aiNodeAnim* channel = anim->mChannels[i];
		channelNodeName = std::string(channel->mNodeName.C_Str());

		// Ignore non-bones
		if (!bindInverseMatrices.count(channelNodeName))
		{
#ifdef PRINT_SKELETAL_INFO
			Log::warning("Animation channel " + channelNodeName + " is not a bone and was therefore ignored.");
#endif

			continue;
		}

		Bone* bone = nullptr;
		meshData.getBoneIndex(channelNodeName, bone);
		BoneTransforms& boneTransforms = bone->boneTransforms;

		// Positions
		for (unsigned int j = 0; j < channel->mNumPositionKeys; ++j)
		{
			boneTransforms.positionStamps.push_back(
				std::pair<double, XMFLOAT3>(
					channel->mPositionKeys[j].mTime,
					SMath::assimpVec3ToDX(channel->mPositionKeys[j].mValue)
					)
			);
		}

		// Rotations
		for (unsigned int j = 0; j < channel->mNumRotationKeys; ++j)
		{
			boneTransforms.rotationStamps.push_back(
				std::pair<double, XMFLOAT4>(
					channel->mRotationKeys[j].mTime,
					SMath::assimpQuatToDX(channel->mRotationKeys[j].mValue)
					)
			);
		}

		// Scalings
		for (unsigned int j = 0; j < channel->mNumScalingKeys; ++j)
		{
			boneTransforms.scaleStamps.push_back(
				std::pair<double, XMFLOAT3>(
					channel->mScalingKeys[j].mTime,
					SMath::assimpVec3ToDX(channel->mScalingKeys[j].mValue)
					)
			);
		}


#ifdef PRINT_SKELETAL_INFO
		Log::write("Channel node name: " + std::string(channel->mNodeName.C_Str()));
#endif
	}
}

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
			aiVector3D uv = submesh->HasTextureCoords(0) ? 
				submesh->mTextureCoords[0][j] : 
				aiVector3D(0, 0, 0);

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
			//assert(submesh->mFaces[j].mNumIndices == 3u);
			if (submesh->mFaces[j].mNumIndices != 3u)
			{
				continue;
			}

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

	// Print some stats
	Log::write(
		"Triangles in " + fileName + ": " + 
		std::to_string(createdMeshData.getVertices().size())
	);

	return createdMeshData;
}

MeshData MeshLoader::loadAnimatedModel(const std::string& fileName, bool flipVerticalUV)
{
	MeshData createdMeshData;

	// Load
	const aiScene* scene = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene)
	{
		Log::error("Could not load mesh: " + std::string(aiGetErrorString()));
		return createdMeshData;
	}

	// Reference to vertices, for later swapping
	std::vector<AnimVertex>& verts = createdMeshData.getAnimVertices();

	// Loop through each submesh
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		unsigned int beforeNumIndices = (unsigned int) createdMeshData.getIndices().size();

		aiMesh* submesh = scene->mMeshes[i];

		// Loop through each vertex
		for (unsigned int j = 0; j < submesh->mNumVertices; ++j)
		{
			// Load data
			aiVector3D pos = submesh->mVertices[j];
			aiVector3D norm = submesh->mNormals[j];
			aiVector3D uv = submesh->HasTextureCoords(0) ?
				submesh->mTextureCoords[0][j] :
				aiVector3D(0, 0, 0);

			if (flipVerticalUV)
				uv.y = 1.0f - uv.y;

			// Create vertex with data
			AnimVertex newAnimVertex{};
			newAnimVertex.pos = XMFLOAT3(pos.x, pos.y, pos.z);
			newAnimVertex.normal = XMFLOAT3(norm.x, norm.y, norm.z);
			newAnimVertex.uv = XMFLOAT2(uv.x, uv.y);
			newAnimVertex.boneWeights = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			newAnimVertex.boneIndices = XMINT4(-1, -1, -1, -1);

			createdMeshData.addAnimVertex(newAnimVertex);
		}

		// Loop through each index
		for (unsigned int j = 0; j < submesh->mNumFaces; ++j)
		{
			//assert(submesh->mFaces[j].mNumIndices == 3u);
			if (submesh->mFaces[j].mNumIndices != 3u)
			{
				continue;
			}

			createdMeshData.addIndex(submesh->mFaces[j].mIndices[0]);
			createdMeshData.addIndex(submesh->mFaces[j].mIndices[1]);
			createdMeshData.addIndex(submesh->mFaces[j].mIndices[2]);
		}

		// Add submesh
		Submesh newSubmesh{};
		newSubmesh.startIndex = beforeNumIndices;
		newSubmesh.numIndices = (unsigned int) createdMeshData.getIndices().size();
		createdMeshData.addSubmesh(newSubmesh);

		// Loop through each bone
		std::map<std::string, XMFLOAT4X4> bindInverseMatrices; // (offsetMatrices)
		for (unsigned int j = 0; j < submesh->mNumBones; ++j)
		{
			aiBone* bone = submesh->mBones[j];

#ifdef PRINT_SKELETAL_INFO
			Log::write("Bone " + std::to_string(j) + " " + std::string(bone->mName.C_Str()));
#endif

			// Save bind inverse matrix offset matrix
			bindInverseMatrices.insert(
				std::pair<std::string, XMFLOAT4X4>(
					bone->mName.C_Str(),
					SMath::assimpMatToDX(bone->mOffsetMatrix)
				)
			);


			Bone newBone{};
			newBone.name = std::string(bone->mName.C_Str());
			newBone.parentIndex = -1;

			// Add weights to bone
			for (unsigned int k = 0; k < bone->mNumWeights; ++k)
			{
				newBone.weights.push_back(
					std::pair<unsigned int, double>(
						bone->mWeights[k].mVertexId,
						bone->mWeights[k].mWeight
					)
				);
			}

			// Add bone
			createdMeshData.addBone(newBone);
		}

#ifdef PRINT_SKELETAL_INFO
		Log::write("\nSkeleton:");
#endif

		// Traverse skeleton hierarchy
		unsigned int globalBoneIndex = 0;
		traverseSkeleton(bindInverseMatrices, createdMeshData, scene->mRootNode, globalBoneIndex, -1, "");

#ifdef PRINT_SKELETAL_INFO
		for (unsigned int k = 0; k < createdMeshData.getSkeleton().size(); ++k)
		{
			Log::write(
				"Bone " + std::to_string(k) + " " + std::string(createdMeshData.getSkeleton()[k].name) +
				"  p: " + std::to_string(createdMeshData.getSkeleton()[k].parentIndex)
			);
		}
#endif

		// Assign weights after swapping
		std::vector<unsigned int> vertexNumBones;
		vertexNumBones.resize(createdMeshData.getAnimVertices().size(), 0);
		for (unsigned int j = 0; j < createdMeshData.getSkeleton().size(); ++j)
		{
			std::vector<std::pair<unsigned int, double>> currentWeights =
				createdMeshData.getSkeleton()[j].weights;

			// Loop through each weight for this bone
			for (unsigned int k = 0; k < currentWeights.size(); ++k)
			{
				unsigned int vertexIndex = currentWeights[k].first;
				float boneWeight = (float)currentWeights[k].second;

				// Distribute weights
				switch (vertexNumBones[vertexIndex])
				{
				case 0:
					verts[vertexIndex].boneWeights.x = boneWeight;
					verts[vertexIndex].boneIndices.x = j;
					break;
				case 1:
					verts[vertexIndex].boneWeights.y = boneWeight;
					verts[vertexIndex].boneIndices.y = j;
					break;
				case 2:
					verts[vertexIndex].boneWeights.z = boneWeight;
					verts[vertexIndex].boneIndices.z = j;
					break;
				case 3:
					verts[vertexIndex].boneWeights.w = boneWeight;
					verts[vertexIndex].boneIndices.w = j;
					break;
				}

				vertexNumBones[vertexIndex]++;
			}
		}

		// Normalize weights so the sum is 1
		for (unsigned int j = 0; j < verts.size(); ++j)
		{
			float totalWeight =
				verts[j].boneWeights.x +
				verts[j].boneWeights.y +
				verts[j].boneWeights.z +
				verts[j].boneWeights.w;

			// Don't divide by 0
			if (abs(totalWeight) <= 0.0001f)
				continue;

			verts[j].boneWeights.x /= totalWeight;
			verts[j].boneWeights.y /= totalWeight;
			verts[j].boneWeights.z /= totalWeight;
			verts[j].boneWeights.w /= totalWeight;
		}

		// Load animation poses
		loadPoses(bindInverseMatrices, createdMeshData, scene);
	}

	
	aiReleaseImport(scene);

	createdMeshData.invertFaces();
	createdMeshData.calculateNormals();

	// Print some stats
	Log::write(
		"Triangles in animated model " + fileName + ": " +
		std::to_string(createdMeshData.getAnimVertices().size())
	);

	return createdMeshData;
}
