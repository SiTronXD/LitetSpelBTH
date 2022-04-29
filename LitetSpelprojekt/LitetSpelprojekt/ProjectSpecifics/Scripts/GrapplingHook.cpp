#include "GrapplingHook.h"
#include "../../Engine/Dev/Log.h"
#include "../../Engine/SMath.h"
#include "../../Engine/GameObject.h"

using namespace DirectX::SimpleMath;

GrapplingHook::GrapplingHook(GameObject& gameObject)
	: Script(gameObject),
	transform(nullptr),
	playerTransform(nullptr)
{
	this->transform = gameObject.getComponent<Transform>();
}

GrapplingHook::~GrapplingHook()
{
}

void GrapplingHook::setPlayerTransform(Transform* playerTransform)
{
	this->playerTransform = playerTransform;
}

void GrapplingHook::init()
{

}

void GrapplingHook::update()
{
	// Update position
	Vector3 newPos = this->playerTransform->getPosition();
	newPos += this->playerTransform->forward() * 3.0f;
	this->transform->setPosition(newPos);

	// Update rotation
	Vector4 rotation = Quaternion::LookRotation(
		this->playerTransform->right(),
		Vector3(0, 1, 0)
	);

	Log::write(std::to_string(rotation.x) + " " +
		std::to_string(rotation.y) + " " +
		std::to_string(rotation.z) + " ");

	this->transform->setRotation(
		rotation
	);
}

void GrapplingHook::onCollisionEnter(GameObject& other)
{
}

void GrapplingHook::onCollisionStay(GameObject& other)
{
}

void GrapplingHook::onCollisionExit(GameObject& other)
{
}
