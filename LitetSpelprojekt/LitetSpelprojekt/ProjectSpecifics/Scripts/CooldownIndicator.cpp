#include "CooldownIndicator.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Time.h"
#include "../../Engine/SMath.h"

using namespace DirectX::SimpleMath;

CooldownIndicator::CooldownIndicator(GameObject& gameObject)
	: Script(gameObject),
	grapplingHook(nullptr),
	transform(nullptr),
	mesh(nullptr),
	percent(1.0f)
{
	this->emptyColor = Vector3(1, 0, 0);
	this->fullColor = Vector3(0, 1, 0);
}

CooldownIndicator::~CooldownIndicator()
{
}

void CooldownIndicator::setup(GrapplingHook* grapplingHook)
{
	this->grapplingHook = grapplingHook;

	this->mesh = Script::getObject().getComponent<AbsoluteMeshComp>();
}

void CooldownIndicator::setPercent(float percent)
{
	this->percent = SMath::clamp(0, 1, percent);
}

void CooldownIndicator::init()
{
	this->transform = Script::getTransform();
}

void CooldownIndicator::update()
{
	// Position
	this->transform->setPosition(
		this->grapplingHook->getTransform()->getPosition() + 
		this->grapplingHook->getTransform()->forward() * (-0.5f + this->percent) +
		this->grapplingHook->getTransform()->up() * 0.85f + 
		this->grapplingHook->getTransform()->right() * -0.55f
	);

	// Rotation
	Vector3 forwardVec = 
		this->grapplingHook->getTransform()->up() +
		-this->grapplingHook->getTransform()->right();
	forwardVec.Normalize();
	this->transform->setRotation(
		forwardVec,
		this->grapplingHook->getTransform()->up()
	);

	// Scale
	this->transform->setScaling(
		Vector3(this->percent, 0.2f, 1)
	);

	// Color
	Vector3 col = Vector3::Lerp(
		this->emptyColor, 
		this->fullColor, 
		this->percent
	);
	this->mesh->setColor(col);
}

void CooldownIndicator::onCollisionEnter(GameObject& other)
{
}

void CooldownIndicator::onCollisionStay(GameObject& other)
{
}

void CooldownIndicator::onCollisionExit(GameObject& other)
{
}
