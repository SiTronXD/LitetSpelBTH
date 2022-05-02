#include "MeshComp.h"
#include "../GameObject.h"

MeshComp::MeshComp(GameObject& object):
	Component(object)
{
}

MeshComp::~MeshComp()
{
}

void MeshComp::setMesh(const std::string& meshName, const std::string& materialName)
{
	this->meshName = meshName;
	this->materialName = materialName;
}
