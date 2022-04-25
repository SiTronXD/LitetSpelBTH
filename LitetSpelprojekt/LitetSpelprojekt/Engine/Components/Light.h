#pragma once

#include "Component.h"
#include "Componentpch.h"

enum class LightType
{
	DIRECTIONAL,
	POINT
};

class Light : public Component
{
private:
	struct DirectionalLightProperties
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 direction;
	} dirLightProps{};

	LightType type;

public:
	Light(GameObject& object);
	virtual ~Light();
};