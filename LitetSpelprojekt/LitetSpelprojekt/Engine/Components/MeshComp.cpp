#include "MeshComp.h"
#include "../GameObject.h"

using namespace DirectX::SimpleMath;

MeshComp::MeshComp(GameObject& object):
	Component(object)
{
	this->setColor(Vector3(1,1,1));
}

MeshComp::~MeshComp()
{
}

void MeshComp::setMesh(const std::string& meshName, const std::string& materialName)
{
	this->meshName = meshName;
	this->materialName = materialName;
}

void MeshComp::setColor(const DirectX::SimpleMath::Vector3& color)
{
	this->color = color;
}
