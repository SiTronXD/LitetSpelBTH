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

	DirectX::SimpleMath::Vector3 forward;
	DirectX::SimpleMath::Vector3 left;
	DirectX::SimpleMath::Vector3 up;
	
	void updateWorldMatrix();
	void updateDirectionalVectors();
public:
	Transform();
	~Transform();

	//Position
	void setPosition(const DirectX::SimpleMath::Vector3& pos);
	void setPosition(float x, float y, float z);

	void move(const DirectX::SimpleMath::Vector3& offSet);
	void move(float x, float y, float z);

	void moveLocal(const DirectX::SimpleMath::Vector3& offSet);
	void moveLocal(float x, float y, float z);

	inline const DirectX::SimpleMath::Vector3& getPosition() const { return this->pos; }

	//Rotation
	void setRotation(const DirectX::SimpleMath::Vector3& rot);
	void setRotation(float x, float y, float z);

	inline const DirectX::SimpleMath::Vector3& getRotation() const { return this->rot; }

	void rotate(const DirectX::SimpleMath::Vector3& offSet);
	void rotate(float x, float y, float z);

	//Scaling
	void setScaling(const DirectX::SimpleMath::Vector3& scale);
	void setScaling(float x, float y, float z);

	void rescale(const DirectX::SimpleMath::Vector3& offSet);
	void rescale(float x, float y, float z);

	inline const DirectX::SimpleMath::Vector3& getScaling() const { return this->scale; }
	
	//Matrix
	void setWorldMatrix(const DirectX::SimpleMath::Matrix& matrix);
	
	inline const DirectX::SimpleMath::Matrix& getWorldMatrix()
	{
		this->updateWorldMatrix();

		return this->worldMatrix;
	}

	void setWorldMatrixIdentity();
	

};

