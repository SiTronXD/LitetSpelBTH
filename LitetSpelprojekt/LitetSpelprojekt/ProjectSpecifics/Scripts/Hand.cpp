#include <Windows.h>
#include <SimpleMath.h>
#include "Hand.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Time.h"
#include "../../Engine/SMath.h"

using namespace DirectX::SimpleMath;

Hand::Hand(GameObject& gameObject)
	: Script(gameObject),
	transform(nullptr),
	targetTransform(nullptr),
	meshComp(nullptr),
	playTimer(MAX_PLAY_TIME)
{
}

Hand::~Hand()
{
}

void Hand::setup(GameObject& targetObject)
{
	this->transform = this->getObject().getComponent<Transform>();
	this->targetTransform = targetObject.getComponent<Transform>();

	this->meshComp = this->getObject().getComponent<MeshComp>();
	this->meshComp->setEnableRender(false);

	this->transform->setScaling(Vector3(1.0f, -1.0f, 1.0f) * 1.0f);
}

void Hand::playAnim()
{
	this->playTimer = 0.0f;
	this->meshComp->setEnableRender(true);
}

void Hand::init()
{
}

void Hand::update()
{
	if (this->playTimer < MAX_PLAY_TIME)
	{
		this->playTimer += Time::getDT();

		// Position
		Vector3 newPos = this->targetTransform->getPosition() +
			this->targetTransform->right() * -6.0f +
			this->targetTransform->up() * (1.5f) +
			this->targetTransform->forward() * -1.5f;
		this->transform->setPosition(newPos);

		// Rotate
		Vector3 tempUp = -this->targetTransform->right() +
			-this->targetTransform->forward() * 0.3f;
		tempUp.Normalize();
		this->transform->setRotation(
			this->targetTransform->up(),
			tempUp
		);

		// Update animation timer
		this->meshComp->setAnimTimer(this->playTimer * 50.0f);

		// Disable rendering hand
		if (this->playTimer >= MAX_PLAY_TIME)
		{
			this->meshComp->setEnableRender(false);
		}
	}
}
