#pragma once

#include "MeshComp.h"

class AbsoluteMeshComp : public MeshComp
{
public:
	AbsoluteMeshComp(GameObject& gameObject);
	virtual ~AbsoluteMeshComp();
};