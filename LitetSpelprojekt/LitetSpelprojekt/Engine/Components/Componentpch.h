#pragma once
#include "Camera.h"
#include "Light.h"
#include "MeshComp.h"
#include "ParticleEmitter.h"
#include "../../ProjectSpecifics/Scripts/Player.h"
#include "Rigidbody.h"
#include "Script.h"
#include "Transform.h"
#include "HookPoint.h"

enum class ObjectTag { UNTAGGED, GROUND, PLAYER, ENEMY, KEY, PORTAL, COUNT };
