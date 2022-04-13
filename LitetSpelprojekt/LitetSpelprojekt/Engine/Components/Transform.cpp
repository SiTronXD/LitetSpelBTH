#include "Transform.h"

void Transform::updateWorldMatrix()
{
    this->worldMatrix = DirectX::XMMatrixTranspose(
        DirectX::XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z) *
        DirectX::XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) *
        DirectX::XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z)
    );
}

void Transform::updateDirectionalVectors()
{
    //Get rotation matrix
    DirectX::SimpleMath::Matrix rotMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);

    //Update them
    this->forward = DirectX::XMVector3TransformCoord(DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 0.0f), rotMatrix);
    this->left = DirectX::XMVector3TransformCoord(DirectX::SimpleMath::Vector4(-1.0f, 0.0f, 0.0f, 0.0f), rotMatrix);
    this->up = DirectX::XMVector3TransformCoord(DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f), rotMatrix);
}

Transform::Transform(GameObject& object):
    Component(object),
    pos(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)),
    rot(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)),
    scale(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f)),
    forward(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f)),
    left(DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f)),
    up(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f))
{
    this->setWorldMatrixIdentity();
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
    this->move(offSet.x * this->left.x, offSet.y * this->up.y, offSet.z * this->forward.z);
}

void Transform::moveLocal(float x, float y, float z)
{
    this->move(x * this->left.x, y * this->up.y, z * this->forward.z);
}

void Transform::setRotation(const DirectX::SimpleMath::Vector3& rot)
{
    this->rot = rot;
}

void Transform::setRotation(float x, float y, float z)
{
    this->rot.x = x;
    this->rot.y = y;
    this->rot.z = z;
}

void Transform::rotate(const DirectX::SimpleMath::Vector3& offSet)
{
    this->updateDirectionalVectors();
    
    this->rot += offSet;
}


void Transform::rotate(float x, float y, float z)
{
    this->updateDirectionalVectors();
    
    this->rot.x += x;
    this->rot.y += y;
    this->rot.z += z;
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

void Transform::setWorldMatrix(const DirectX::SimpleMath::Matrix& matrix)
{
    this->worldMatrix = matrix;
}

void Transform::setWorldMatrixIdentity()
{
    this->worldMatrix = DirectX::SimpleMath::Matrix(
                DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.0f),
                DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f),
                DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 0.0f),
                DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f)
        );
}
