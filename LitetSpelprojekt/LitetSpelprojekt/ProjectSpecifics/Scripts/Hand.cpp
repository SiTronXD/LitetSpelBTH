#include <Windows.h>
#include <SimpleMath.h>
#include "Hand.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Time.h"
#include "../../Engine/SMath.h"
#include "GrapplingHook.h"

using namespace DirectX::SimpleMath;

float Hand::getPositionOffset()
{
	float t = ((this->playTimer / MAX_PLAY_TIME) * 2.0f - 1.0f) * 1.6f + 0.3f;

	// Visualized curve here:
	// https://www.desmos.com/calculator/ziv2qhu63g?lang=sv-SE
	const float H = -3.15f;
	const float I = 0.0f;
	const float J = 2.5f;
	const float K = 0.0f;
	const float L = 0.5f;
	float offset =
		H * t * t * t * t +
		//I * t * t * t +
		J * t * t +
		//K * t +
		L;

	return offset;
}

Hand::Hand(GameObject& gameObject)
	: Script(gameObject),
	transform(nullptr),
	targetTransform(nullptr),
	grapplingHook(nullptr),
	meshComp(nullptr),
	playTimer(MAX_PLAY_TIME)
{
}

Hand::~Hand()
{
}

void Hand::setup(GameObject& grapplingHookObject)
{
	this->transform = this->getObject().getComponent<Transform>();
	this->targetTransform = grapplingHookObject.getComponent<Transform>();
	this->grapplingHook = grapplingHookObject.getComponent<GrapplingHook>();

	this->meshComp = this->getObject().getComponent<MeshComp>();
	this->meshComp->setEnableRender(false);

	this->transform->setScaling(Vector3(1.0f, -1.0f, 1.0f));
}

void Hand::playAnim()
{
	this->playTimer = -0.2f;
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
		
		float offset = this->getPositionOffset();

		// Position
		Vector3 newPos = this->targetTransform->getPosition() +
			this->targetTransform->right() * -5.5f +
			this->targetTransform->up() * (0.5f + offset) +
			this->targetTransform->forward() * -1.5f;
		newPos -= this->grapplingHook->getSwayDeltaPos() * 0.6f;
		this->transform->setPosition(newPos);

		// Rotate
		Vector3 tempUp = -this->targetTransform->right() +
			-this->targetTransform->forward() * 0.3f;
		tempUp.Normalize();
		this->transform->setRotation(
			this->targetTransform->up() + 
				this->targetTransform->forward() * 0.2f +
				this->targetTransform->right() * 0.2f,
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
