#pragma once

#include <string>
#include "Component.h"
#include "Componentpch.h"

class AbsoluteMeshComp : public Component
{
private:
	std::string meshName;
	std::string materialName;

public:
	AbsoluteMeshComp(GameObject& gameObject);
	virtual ~AbsoluteMeshComp();

	void setMesh(
		const std::string& meshName,
		const std::string& materialName);

	inline const std::string& getMeshName() { return this->meshName; }
	inline const std::string& getMaterialName() { return this->materialName; }
};