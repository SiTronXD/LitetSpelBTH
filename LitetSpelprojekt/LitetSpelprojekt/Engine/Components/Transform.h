#pragma once
#include "Component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>


class Transform : public Component
{
private:
	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector3 rot;
	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Matrix worldMatrix;
	
	void updateWorldMatrix();
public:
	Transform();
	~Transform();

	//Position
	void setPosition(const DirectX::SimpleMath::Vector3& pos);
	void setPosition(float x, float y, float z);

	void adjustPosition(const DirectX::SimpleMath::Vector3& offSet);
	void adjustPosition(float x, float y, float z);

	const DirectX::SimpleMath::Vector3& getPosition() const;

	//Rotation
	void setRotation(const DirectX::SimpleMath::Vector3& rot);
	void setRotation(float x, float y, float z);

	const DirectX::SimpleMath::Vector3& getRotation() const;

	void adjustRotation(const DirectX::SimpleMath::Vector3& offSet);
	void adjustRotation(float x, float y, float z);

	//Scaling
	void setScaling(const DirectX::SimpleMath::Vector3& scale);
	void setScaling(float x, float y, float z);

	void adjustScaling(const DirectX::SimpleMath::Vector3& offSet);
	void adjustScaling(float x, float y, float z);

	const DirectX::SimpleMath::Vector3& getScaling() const;
	
	//Matrix
	void setWorldMatrix(const DirectX::SimpleMath::Matrix& matrix);
	const DirectX::SimpleMath::Matrix& getWorldMatrix() const;
	void setWorldMatrixIdentity();
	

};

