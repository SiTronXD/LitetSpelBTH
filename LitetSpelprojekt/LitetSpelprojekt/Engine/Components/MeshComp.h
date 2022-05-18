#pragma once

#include <string>
#include <Windows.h>
#include <SimpleMath.h>
#include "Component.h"

class MeshComp : public Component
{
private:
	std::string meshName;
	std::string materialName;
	std::string pixelShaderName;

	DirectX::SimpleMath::Vector3 color;

	float animTimer;

	bool shouldShade;
	bool castShadow;
	bool renderMesh;

public:
	MeshComp(GameObject& object);
	virtual ~MeshComp();

	void setMesh(
		const std::string& meshName,
		const std::string& materialName
	);
	void setColor(
		const DirectX::SimpleMath::Vector3& color
	);
	void setPixelShaderName(const std::string& pixelShaderName);
	void setShouldShade(bool shade);
	void setCastShadow(bool cast);
	void setAnimTimer(float animTimer);
	void setEnableRender(bool renderMesh);

	inline const std::string& getMeshName() { return this->meshName; }
	inline const std::string& getMaterialName() { return this->materialName; }
	inline const std::string& getPixelShaderName() { return this->pixelShaderName; }
	inline const DirectX::SimpleMath::Vector3& getColor() { return this->color; }
	inline const bool getShouldShade() { return this->shouldShade; }
	inline const bool getCastShadow() { return this->castShadow; }
	inline const bool getShouldRenderMesh() { return this->renderMesh; }
	inline const float getAnimTimer() { return this->animTimer; }
};