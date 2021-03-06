#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Component.h"
#include "Componentpch.h"

class Camera : public Component
{
private:
	DirectX::SimpleMath::Matrix viewMatrix;
	DirectX::SimpleMath::Matrix projectionMatrix;
	DirectX::SimpleMath::Matrix invProjectionMatrix;

	float aspectRatio;
	float fovPercent;

	void updateViewMatrix();
	void updateProjectionMatrix();

public:
	Camera(GameObject& object);
	virtual ~Camera();

	void updateAspectRatio(float aspectRatio);
	void updateFovPercent(float fovPercent);

	inline const DirectX::SimpleMath::Matrix& getViewMatrix() 
	{
		this->updateViewMatrix();
		return this->viewMatrix; 
	}
	inline const DirectX::SimpleMath::Matrix& getProjectionMatrix() { return this->projectionMatrix; }
	inline const DirectX::SimpleMath::Matrix& getInvProjectionMatrix() { return this->invProjectionMatrix; }
};