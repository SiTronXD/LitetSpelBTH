#include "Camera.h"
#include "../GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Camera::updateViewMatrix()
{
	this->viewMatrix = Matrix::CreateLookAt(
		this->getTransform()->getPosition(),
		this->getTransform()->getPosition() + this->getTransform()->forward(),
		Vector3(0, 1, 0)
	);
}

Camera::Camera(GameObject& object):
	Component(object)
{
	this->updateViewMatrix();
	this->updateAspectRatio(16.0f / 9.0f);
}

Camera::~Camera()
{
}

void Camera::updateAspectRatio(float aspectRatio)
{
	this->projectionMatrix = Matrix::CreatePerspectiveFieldOfView(
		90.0f,
		aspectRatio,
		0.1f,
		100.0f
	);
}
