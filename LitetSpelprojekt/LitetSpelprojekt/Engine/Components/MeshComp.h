#pragma once

#include "Component.h"
#include "../Graphics/Mesh.h"

class Renderer;

class MeshComp : public Component
{
private:
	Mesh mesh;

public:
	MeshComp(Renderer& renderer);
	virtual ~MeshComp();

	inline Mesh& getMesh() { return this->mesh; }
};