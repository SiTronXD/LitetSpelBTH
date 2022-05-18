#include "SMath.h"

const float SMath::PI = 3.141592f;

using namespace DirectX::SimpleMath;

void SMath::assimpToMat(
	aiMatrix4x4& assimpMat, 
	DirectX::SimpleMath::Matrix& outMat)
{
	outMat._11 = assimpMat.a1;
	outMat._21 = assimpMat.a2;
	outMat._31 = assimpMat.a3;
	outMat._41 = assimpMat.a4;

	outMat._12 = assimpMat.b1;
	outMat._22 = assimpMat.b2;
	outMat._32 = assimpMat.b3;
	outMat._42 = assimpMat.b4;

	outMat._13 = assimpMat.c1;
	outMat._23 = assimpMat.c2;
	outMat._33 = assimpMat.c3;
	outMat._43 = assimpMat.c4;

	outMat._14 = assimpMat.d1;
	outMat._24 = assimpMat.d2;
	outMat._34 = assimpMat.d3;
	outMat._44 = assimpMat.d4;
}

float SMath::clamp(const float& value, const float& min, const float& max)
{
	if (value < min) return min;
	else if (value > max) return max;

	return value;
}

float SMath::lerp(const float& v1, const float& v2, float t)
{
	t = SMath::clamp(t, 0.0f, 1.0f);

	return (1.0f - t) * v1 + t * v2;
}

DirectX::XMFLOAT4X4 SMath::assimpMatToDX(const aiMatrix4x4& assimpMat)
{
	DirectX::XMFLOAT4X4 mat;
	/*mat._11 = assimpMat.a1;
	mat._12 = assimpMat.a2;
	mat._13 = assimpMat.a3;
	mat._14 = assimpMat.a4;
	mat._21 = assimpMat.b1;
	mat._22 = assimpMat.b2;
	mat._23 = assimpMat.b3;
	mat._24 = assimpMat.b4;
	mat._31 = assimpMat.c1;
	mat._32 = assimpMat.c2;
	mat._33 = assimpMat.c3;
	mat._34 = assimpMat.c4;
	mat._41 = assimpMat.d1;
	mat._42 = assimpMat.d2;
	mat._43 = assimpMat.d3;
	mat._44 = assimpMat.d4;*/

	mat._11 = assimpMat.a1;
	mat._12 = assimpMat.b1;
	mat._13 = assimpMat.c1;
	mat._14 = assimpMat.d1;
	mat._21 = assimpMat.a2;
	mat._22 = assimpMat.b2;
	mat._23 = assimpMat.c2;
	mat._24 = assimpMat.d2;
	mat._31 = assimpMat.a3;
	mat._32 = assimpMat.b3;
	mat._33 = assimpMat.c3;
	mat._34 = assimpMat.d3;
	mat._41 = assimpMat.a4;
	mat._42 = assimpMat.b4;
	mat._43 = assimpMat.c4;
	mat._44 = assimpMat.d4;

	return mat;
}

DirectX::XMFLOAT3 SMath::assimpVec3ToDX(const aiVector3D& assimpVec)
{
	return DirectX::XMFLOAT3(assimpVec.x, assimpVec.y, assimpVec.z);
}

DirectX::XMFLOAT4 SMath::assimpQuatToDX(const aiQuaternion& assimpQuat)
{
	return DirectX::XMFLOAT4(assimpQuat.x, assimpQuat.y, assimpQuat.z, assimpQuat.w);
}