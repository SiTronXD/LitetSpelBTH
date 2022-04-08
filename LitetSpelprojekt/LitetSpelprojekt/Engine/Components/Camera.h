#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include "Component.h"

class Camera : public Component
{
private:
	DirectX::SimpleMath::Matrix viewMatrix;
	DirectX::SimpleMath::Matrix projectionMatrix;

public:
	Camera();
	virtual ~Camera();

	inline const DirectX::SimpleMath::Matrix& getViewMatrix() { return this->viewMatrix; }
	inline const DirectX::SimpleMath::Matrix& getProjectionMatrix() { return this->projectionMatrix; }
};