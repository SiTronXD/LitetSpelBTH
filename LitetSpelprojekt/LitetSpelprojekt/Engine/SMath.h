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
};