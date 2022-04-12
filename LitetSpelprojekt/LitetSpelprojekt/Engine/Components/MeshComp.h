#pragma once

#include "Component.h"
#include "../Graphics/Mesh.h"

class Renderer;

class MeshComp : public Component
{
private:
	std::string meshName;
	std::string materialName;

public:
	MeshComp(
		const std::string& meshName, 
		const std::string& materialName);
	virtual ~MeshComp();

	inline const std::string& getMeshName() { return this->meshName; }
	inline const std::string& getMaterialName() { return this->materialName; }
};