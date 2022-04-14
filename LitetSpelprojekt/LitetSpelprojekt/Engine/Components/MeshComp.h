#pragma once

#include "Component.h"
#include "../Graphics/Mesh.h"
#include "Componentpch.h"

class Renderer;

class MeshComp : public Component
{
private:
	std::string meshName;
	std::string materialName;

public:
	MeshComp(GameObject& object);
	virtual ~MeshComp();

	void setMesh(
		const std::string& meshName,
		const std::string& materialName);

	inline const std::string& getMeshName() { return this->meshName; }
	inline const std::string& getMaterialName() { return this->materialName; }
};