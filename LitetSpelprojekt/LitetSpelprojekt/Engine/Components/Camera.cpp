#include "Camera.h"
#include "../GameObject.h"
#include "../SMath.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Camera::updateViewMatrix()
{
	this->viewMatrix = Matrix::CreateLookAt(
		this->getTransform()->getPosition(),
		this->getTransform()->getPosition() + this->getTransform()->forward(),
		this->getTransform()->up() // Could change if precision is off
	);
}

void Camera::updateProjectionMatrix()
{
	this->projectionMatrix = Matrix::CreatePerspectiveFieldOfView(
		SMath::PI * 0.5f * this->fovPercent,
		this->aspectRatio,
		0.1f,
		300.0f
	);

	// Reversed Z, infinite far plane
	/*float f = 1.0f / tan(SMath::PI * 0.5f * 0.5f);
	this->projectionMatrix = Matrix(
		f / aspectRatio,	0.0f,	0.0f,	0.0f,
		0.0f,				f,		0.0f,	0.0f,
		0.0f,				0.0f,	0.0f,	-1.0f,
		0.0f,				0.0f,	0.1f,	0.0f
	);*/


	this->invProjectionMatrix = this->projectionMatrix.Invert();
}

Camera::Camera(GameObject& object):
	Component(object), aspectRatio(16.0f / 9.0f), fovPercent(1.0f)
{
	this->updateViewMatrix();
	this->updateAspectRatio(16.0f / 9.0f);
}

Camera::~Camera()
{
}

void Camera::updateAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	this->updateProjectionMatrix();
}

void Camera::updateFovPercent(float fovPercent)
{
	this->fovPercent = fovPercent;
	this->updateProjectionMatrix();
}
