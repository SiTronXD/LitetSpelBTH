#pragma once
#include "Component.h"
#include <d3d11.h>
#include <DirectXMath.h>

namespace DX = DirectX;

class Transform : public Component
{
private:
	DX::XMVECTOR posVector;
	DX::XMVECTOR rotVector;
	DX::XMVECTOR scaleVector;
	DX::XMFLOAT3 pos;
	DX::XMFLOAT3 rot;
	DX::XMFLOAT3 scale;
	DX::XMMATRIX worldMatrix;

	void updateWorldMatrix();
public:
	Transform();
	~Transform();

	//Position
	void setPosition(const DX::XMVECTOR& pos);
	void setPosition(const DX::XMFLOAT3& pos);
	void setPosition(float x, float y, float z);

	void adjustPosition(const DX::XMVECTOR& offSet);
	void adjustPosition(const DX:: XMFLOAT3& offSet);
	void adjustPosition(float x, float y, float z);

	const DX::XMVECTOR& getPositionVector() const;
	const DX::XMFLOAT3& getPositionFloat3() const;
	float getPositionX() const;
	float getPositionY() const;
	float getPositionZ() const;

	//Rotation
	void setRotation(const DX::XMVECTOR& rot);
	void setRotation(const DX::XMFLOAT3& rot);
	void setRotation(float x, float y, float z);

	const DX::XMVECTOR& getRotationVector() const;
	const DX::XMFLOAT3& getRotationFloat3() const;
	float getRotationX() const;
	float getRotationY() const;
	float getRotationZ() const;

	void adjustRotation(const DX::XMVECTOR& offSet);
	void adjustRotation(const DX::XMFLOAT3& offSet);
	void adjustRotation(float x, float y, float z);

	//Scaling
	void setScaling(const DX::XMVECTOR& scale);
	void setScaling(const DX::XMFLOAT3& scale);
	void setScaling(float x, float y, float z);

	void adjustScaling(const DX::XMVECTOR& offSet);
	void adjustScaling(const DX::XMFLOAT3& offSet);
	void adjustScaling(float x, float y, float z);

	const DX::XMVECTOR& getScalingVector() const;
	const DX::XMFLOAT3& getScalingFloat3() const;
	float getScalingX() const;
	float getScalingY() const;
	float getScalingZ() const;

	//Matrix
	void setWorldMatrix(DX::XMMATRIX& matrix);
	const DX::XMMATRIX& getWorldMatrix() const;
	void setWorldMatrixIdentity();
	

};

