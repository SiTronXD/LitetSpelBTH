#include "Transform.h"

void Transform::updateWorldMatrix()
{
    this->worldMatrix = DirectX::XMMatrixTranspose(
        DirectX::XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z) *
        DirectX::XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) *
        DirectX::XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z)
    );
}

Transform::Transform()
    :pos(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)),
    rot(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)),
    scale(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f))
{
    this->setWorldMatrixIdentity();
}

Transform::~Transform()
{
}

void Transform::setPosition(const DirectX::SimpleMath::Vector3& pos)
{
    this->pos = pos;
    void updateWorldMatrix();
}

void Transform::setPosition(float x, float y, float z)
{
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
    void updateWorldMatrix();
}

void Transform::adjustPosition(const DirectX::SimpleMath::Vector3& offSet)
{  
    this->pos += offSet;
    void updateWorldMatrix();
}

void Transform::adjustPosition(float x, float y, float z)
{
    this->pos.x += x;
    this->pos.y += y;
    this->pos.z += z;
    void updateWorldMatrix();
}

const DirectX::SimpleMath::Vector3& Transform::getPosition() const
{
    return this->pos;
}

void Transform::setRotation(const DirectX::SimpleMath::Vector3& rot)
{
    this->rot = rot;
    void updateWorldMatrix();
}

void Transform::setRotation(float x, float y, float z)
{
    this->rot.x = x;
    this->rot.y = y;
    this->rot.z = z;
    void updateWorldMatrix();
}

const DirectX::SimpleMath::Vector3& Transform::getRotation() const
{
    return this->rot;
}

void Transform::adjustRotation(const DirectX::SimpleMath::Vector3& offSet)
{
    this->rot += offSet;
    void updateWorldMatrix();
}


void Transform::adjustRotation(float x, float y, float z)
{
    this->rot.x += x;
    this->rot.y += y;
    this->rot.z += z;
    void updateWorldMatrix();
}

void Transform::setScaling(const DirectX::SimpleMath::Vector3& scale)
{
    this->scale = scale;
    void updateWorldMatrix();
}


void Transform::setScaling(float x, float y, float z)
{
    this->scale.x = x;
    this->scale.y = y;
    this->scale.z = z;
    void updateWorldMatrix();
}

void Transform::adjustScaling(const DirectX::SimpleMath::Vector3& offSet)
{
    this->scale += offSet;
    void updateWorldMatrix();
}


void Transform::adjustScaling(float x, float y, float z)
{
    this->scale.x += x;
    this->scale.y += y;
    this->scale.z += z;
    void updateWorldMatrix();
}

const DirectX::SimpleMath::Vector3& Transform::getScaling() const
{
    return this->scale;
}


void Transform::setWorldMatrix(const DirectX::SimpleMath::Matrix& matrix)
{
    this->worldMatrix = matrix;
    void updateWorldMatrix();
}

const DirectX::SimpleMath::Matrix& Transform::getWorldMatrix() const
{
    return this->worldMatrix;
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
