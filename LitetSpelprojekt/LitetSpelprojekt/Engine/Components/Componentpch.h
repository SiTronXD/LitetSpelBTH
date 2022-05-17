#pragma once
#include "Script.h"
#include "Camera.h"
#include "Light.h"
#include "AbsoluteMeshComp.h"
#include "BackgroundMeshComp.h"
#include "MeshComp.h"
#include "ParticleEmitter.h"
#include "Rigidbody.h"
#include "Transform.h"

enum class ObjectTag { UNTAGGED, GROUND, PLAYER, ENEMY, KEY, PORTAL, COUNT };
