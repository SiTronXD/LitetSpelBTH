#include "MeshComp.h"
#include "../Graphics/Renderer.h"

MeshComp::MeshComp(
	const std::string& meshName, const std::string& materialName)
	: meshName(meshName), materialName(materialName)
{
}

MeshComp::~MeshComp()
{
}
