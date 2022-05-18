#pragma once

#include <Windows.h>
#include <SimpleMath.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

class SMath
{
private:
public:
	static const float PI;

	static void assimpToMat(
		aiMatrix4x4& assimpMat, 
		DirectX::SimpleMath::Matrix& outMat
	);

	static float clamp(
		const float& value, 
		const float& min, 
		const float& max
	);
  
  static float lerp(
		const float& v1,
		const float& v2,
		float t
	);
  
	static DirectX::XMFLOAT4X4 assimpMatToDX(const aiMatrix4x4& assimpMat);
	static DirectX::XMFLOAT3 assimpVec3ToDX(const aiVector3D& assimpVec);
	static DirectX::XMFLOAT4 assimpQuatToDX(const aiQuaternion& assimpQuat);
};