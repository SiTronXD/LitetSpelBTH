#pragma once
#include "Component.h"
#include <d3d11.h>
#include <SimpleMath.h>
#include "Componentpch.h"

class Transform : public Component
{
private:
	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector3 rot;
	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Vector4 rotQuat;
	DirectX::SimpleMath::Matrix worldMatrix;

	DirectX::SimpleMath::Vector3 rightVec;
	DirectX::SimpleMath::Vector3 upVec;
	DirectX::SimpleMath::Vector3 forwardVec;
	
	bool useVectorRotation;
	bool useRenderPos;

	void updateWorldMatrix();
	void updateDirectionalVectors();
public:
	Transform(GameObject& object);
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
	void setRotation(
		const DirectX::SimpleMath::Vector3& newForward,
		const DirectX::SimpleMath::Vector3& worldUp
	);
	void setRotation(const DirectX::SimpleMath::Vector4& quat);
	void setRotation(float x, float y, float z);

	inline const DirectX::SimpleMath::Vector3& getRotation() const { return this->rot; }
	inline const DirectX::SimpleMath::Vector4& getRotationQuat() const { return this->rotQuat; }

	void rotate(const DirectX::SimpleMath::Vector3& offSet);
	void rotate(float x, float y, float z);

	//Scaling
	void setScaling(const DirectX::SimpleMath::Vector3& scale);
	void setScaling(float x, float y, float z);

	void rescale(const DirectX::SimpleMath::Vector3& offSet);
	void rescale(float x, float y, float z);

	inline const DirectX::SimpleMath::Vector3& getScaling() const { return this->scale; }
	
	// Local vectors
	inline const DirectX::SimpleMath::Vector3& right() const { return this->rightVec; }
	inline const DirectX::SimpleMath::Vector3& up() const { return this->upVec; }
	inline const DirectX::SimpleMath::Vector3& forward() const { return this->forwardVec; }
	
	inline const DirectX::SimpleMath::Matrix& getWorldMatrix()
	{
		this->updateWorldMatrix();
		return this->worldMatrix;
	}
};

