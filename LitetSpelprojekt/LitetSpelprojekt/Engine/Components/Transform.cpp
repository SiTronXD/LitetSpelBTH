#include "Transform.h"

void Transform::updateWorldMatrix()
{
    // Quaternion rotation
    if (this->rotQuat.x != 0.0f ||
        this->rotQuat.y != 0.0f ||
        this->rotQuat.z != 0.0f ||
        this->rotQuat.w != 0.0f)
    {
        this->worldMatrix =
            DirectX::XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z) *
            DirectX::XMMatrixRotationQuaternion(this->rotQuat) *
            DirectX::XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
    }
    // Euler angles
    else
    {
        this->worldMatrix =
            DirectX::XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z) *
            DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(this->rot.x), DirectX::XMConvertToRadians(this->rot.y), DirectX::XMConvertToRadians(this->rot.z)) *
            DirectX::XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
    }
}

void Transform::updateDirectionalVectors()
{
    this->updateWorldMatrix();

    this->rightVec = this->worldMatrix.Right();
    this->upVec = this->worldMatrix.Up();
    this->forwardVec = this->worldMatrix.Forward();
}

Transform::Transform(GameObject& object):
    Component(object),
    pos(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)),
    rot(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)),
    scale(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f))
{
    this->updateDirectionalVectors();
}

Transform::~Transform()
{
}

void Transform::setPosition(const DirectX::SimpleMath::Vector3& pos)
{
    this->pos = pos;
}

void Transform::setPosition(float x, float y, float z)
{
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
}

void Transform::move(const DirectX::SimpleMath::Vector3& offSet)
{  
    this->pos += offSet;
}

void Transform::move(float x, float y, float z)
{
    this->pos.x += x;
    this->pos.y += y;
    this->pos.z += z;
}

void Transform::moveLocal(const DirectX::SimpleMath::Vector3& offSet)
{
    this->move(offSet.x * this->rightVec + offSet.y * this->upVec + offSet.z * this->forwardVec);
}

void Transform::moveLocal(float x, float y, float z)
{
    this->move(x * this->rightVec + y * this->upVec + z * this->forwardVec);
}

void Transform::setRotation(const DirectX::SimpleMath::Vector3& rot)
{
    this->rot = rot;
    this->updateDirectionalVectors();
}

void Transform::setRotation(const DirectX::SimpleMath::Vector4& quat)
{
    this->rotQuat = quat;
}

void Transform::setRotation(float x, float y, float z)
{
    this->rot.x = x;
    this->rot.y = y;
    this->rot.z = z;
    this->updateDirectionalVectors();
}

void Transform::rotate(const DirectX::SimpleMath::Vector3& offSet)
{
    this->rot += offSet;
    this->updateDirectionalVectors();
}


void Transform::rotate(float x, float y, float z)
{
    this->rot.x += x;
    this->rot.y += y;
    this->rot.z += z;
    this->updateDirectionalVectors();
}

void Transform::setScaling(const DirectX::SimpleMath::Vector3& scale)
{
    this->scale = scale;
}


void Transform::setScaling(float x, float y, float z)
{
    this->scale.x = x;
    this->scale.y = y;
    this->scale.z = z;
}

void Transform::rescale(const DirectX::SimpleMath::Vector3& offSet)
{
    this->scale += offSet;
}


void Transform::rescale(float x, float y, float z)
{
    this->scale.x += x;
    this->scale.y += y;
    this->scale.z += z;
}
