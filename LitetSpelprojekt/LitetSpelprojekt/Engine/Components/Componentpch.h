#pragma once
#include "Camera.h"
#include "Collider.h"
#include "Light.h"
#include "MeshComp.h"
#include "ParticleEmitter.h"
#include "../../ProjectSpecifics/Scripts/Player.h"
#include "Rigidbody.h"
#include "Script.h"
#include "Transform.h"

enum class ObjectTag { UNTAGGED, GROUND, PLAYER, ENEMY, KEY, PORTAL, COUNT };
