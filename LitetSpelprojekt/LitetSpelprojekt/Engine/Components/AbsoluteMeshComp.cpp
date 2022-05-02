#include "AbsoluteMeshComp.h"

AbsoluteMeshComp::AbsoluteMeshComp(GameObject& gameObject)
	: Component(gameObject)
{
}

AbsoluteMeshComp::~AbsoluteMeshComp()
{
}

void AbsoluteMeshComp::setMesh(const std::string& meshName, const std::string& materialName)
{
	this->meshName = meshName;
	this->materialName = materialName;
}
