#include <Windows.h>
#include <SimpleMath.h>
#include "Hand.h"
#include "../../Engine/GameObject.h"

using namespace DirectX::SimpleMath;

Hand::Hand(GameObject& gameObject)
	: Script(gameObject)
{
}

Hand::~Hand()
{
}

void Hand::setup(GameObject& targetObject)
{
	this->transform = this->getObject().getComponent<Transform>();
	this->targetTransform = targetObject.getComponent<Transform>();

	this->transform->setScaling(Vector3(0.02f, 0.02f, 0.02f));
}

void Hand::init()
{
}

void Hand::update()
{
	Vector3 newPos = this->targetTransform->getPosition() +
		this->targetTransform->forward() * 5.0f;

	Log::write("HandPos: " + 
		std::to_string(newPos.x) + " " + 
		std::to_string(newPos.y) + " " + 
		std::to_string(newPos.z));

	this->transform->setPosition(newPos);
}
