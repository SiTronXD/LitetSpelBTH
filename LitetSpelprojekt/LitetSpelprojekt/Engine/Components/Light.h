#pragma once

#include "Component.h"
#include "Componentpch.h"
#include "../Graphics/DSV.h"
#include "../Graphics/Texture.h"
#include "../Graphics/ConstantBuffer.h"

class VertexShader;
class Resources;
class Renderer;
class Scene;

enum class LightType
{
	DIRECTIONAL,
	POINT
};

class Light : public Component
{
private:
	static const unsigned int SHADOW_MAP_SIZE = 512;

	struct LightBufferData
	{
		DirectX::XMFLOAT4X4 vpMatrix;
	} lightBufferStruct{};

	struct DirectionalLightBufferData
	{
		DirectX::XMFLOAT3 direction;
		float padding;
	} dirLightBufferStruct{};

	DirectX::SimpleMath::Matrix viewMatrix;
	DirectX::SimpleMath::Matrix projectionMatrix;

	D3D11_VIEWPORT shadowMapViewport;

	LightType type;

	Texture* shadowMapDepthTexture;
	DSV shadowMapDSV;

	ConstantBuffer* lightBuffer;
	ConstantBuffer* directionalLightBuffer;

	VertexShader* shadowMapVS;

	Resources* resources;
	Renderer* renderer;

public:
	Light(GameObject& object);
	virtual ~Light();

	void init(Resources& resources, Renderer& renderer);

	void render(Scene& scene);

	void updateDirection(DirectX::SimpleMath::Vector3 newDir);

	inline Texture& getShadowMapTexture() { return *this->shadowMapDepthTexture; }
	inline ConstantBuffer& getLightBuffer() { return *this->lightBuffer; }
	inline ConstantBuffer& getDirLightBuffer() { return *this->directionalLightBuffer; }
};