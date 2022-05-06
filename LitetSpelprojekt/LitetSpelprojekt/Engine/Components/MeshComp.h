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

	DirectX::SimpleMath::Vector3 color;

	bool shouldShade;
	bool castShadow;

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
	void setShouldShade(bool shade);
	void setCastShadow(bool cast);

	inline const std::string& getMeshName() { return this->meshName; }
	inline const std::string& getMaterialName() { return this->materialName; }
	inline const DirectX::SimpleMath::Vector3& getColor() { return this->color; }
	inline const bool getShouldShade() { return this->shouldShade; }
	inline const bool getCastShadow() { return this->castShadow; }
};