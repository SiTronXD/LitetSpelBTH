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
