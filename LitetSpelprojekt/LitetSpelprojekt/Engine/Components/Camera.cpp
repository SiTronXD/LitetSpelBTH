#include "Camera.h"
#include "../GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(GameObject& object):
	Component(object)
{
	
	this->viewMatrix = Matrix::CreateLookAt(
		Vector3(1.0f, 0.5f, 1.0f),
		Vector3(0, 0, 0),
		Vector3(0, 1, 0)
	);

	this->projectionMatrix = Matrix::CreatePerspectiveFieldOfView(
		90.0f,
		16.0f / 9.0f,
		0.1f,
		100.0f
	);
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
