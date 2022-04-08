#include "Transform.h"

void Transform::updateWorldMatrix()
{
    this->worldMatrix = DX::XMMatrixTranspose(
        DX::XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z) *
        DX::XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) *
        DX::XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z)
    );
}

Transform::Transform()
    :posVector(DX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
    rotVector(DX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
    scaleVector(DX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f)),
    pos(0.0f, 0.0f, 0.0f), rot(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f),
    worldMatrix(DX::XMMatrixIdentity())
{
}

Transform::~Transform()
{
}

void Transform::setPosition(const DX::XMVECTOR& pos)
{
    DX::XMStoreFloat3(&this->pos, pos);
    this->posVector = pos;
    void updateWorldMatrix();
}

void Transform::setPosition(const DX::XMFLOAT3& pos)
{
    this->pos = pos;
    this->posVector = DX::XMLoadFloat3(&this->pos);
    void updateWorldMatrix();
}

void Transform::setPosition(float x, float y, float z)
{
    this->pos = DX::XMFLOAT3(x, y, z);
    this->posVector = DX::XMLoadFloat3(&this->pos);
    void updateWorldMatrix();
}

void Transform::adjustPosition(const DX::XMVECTOR& offSet)
{  
    this->posVector = DX::XMVectorAdd(this->posVector, offSet);
    XMStoreFloat3(&this->pos, this->posVector);
    void updateWorldMatrix();
}

void Transform::adjustPosition(const DX::XMFLOAT3& offSet)
{
    this->pos.x += offSet.x;
    this->pos.y += offSet.y;
    this->pos.z += offSet.z;

    this->posVector = DX::XMLoadFloat3(&this->pos);
    void updateWorldMatrix();
}

void Transform::adjustPosition(float x, float y, float z)
{
    this->pos.x += x;
    this->pos.y += y;
    this->pos.z += z;

    this->posVector = DX::XMLoadFloat3(&this->pos);
    void updateWorldMatrix();
}

const DX::XMVECTOR& Transform::getPositionVector() const
{
    return this->posVector;
}

const DX::XMFLOAT3& Transform::getPositionFloat3() const
{
    return this->pos;
}

float Transform::getPositionX() const
{
    return this->pos.x;
}

float Transform::getPositionY() const
{
    return this->pos.y;
}

float Transform::getPositionZ() const
{
    return this->pos.z;
}

void Transform::setRotation(const DX::XMVECTOR& rot)
{
    DX::XMStoreFloat3(&this->rot, rot);
    this->rotVector = rot;
    void updateWorldMatrix();
}

void Transform::setRotation(const DX::XMFLOAT3& rot)
{
    this->rot = rot;
    this->rotVector = DX::XMLoadFloat3(&this->rot);
    void updateWorldMatrix();
}

void Transform::setRotation(float x, float y, float z)
{
    this->rot = DX::XMFLOAT3(x, y, z);
    this->rotVector = DX::XMLoadFloat3(&this->rot);
    void updateWorldMatrix();
}

const DX::XMVECTOR& Transform::getRotationVector() const
{
    return this->rotVector;
}

const DX::XMFLOAT3& Transform::getRotationFloat3() const
{
    return this->rot;
}

float Transform::getRotationX() const
{
    return this->rot.x;
}

float Transform::getRotationY() const
{
    return this->rot.y;
}

float Transform::getRotationZ() const
{
    return this->rot.z;
}

void Transform::adjustRotation(const DX::XMVECTOR& offSet)
{
    this->rotVector = DX::XMVectorAdd(this->rotVector, offSet);
    XMStoreFloat3(&this->rot, this->rotVector);
    void updateWorldMatrix();
}

void Transform::adjustRotation(const DX::XMFLOAT3& offSet)
{
    this->rot.x += offSet.x;
    this->rot.y += offSet.y;
    this->rot.z += offSet.z;

    this->rotVector = DX::XMLoadFloat3(&this->rot);
    void updateWorldMatrix();
}

void Transform::adjustRotation(float x, float y, float z)
{
    this->rot.x += x;
    this->rot.y += y;
    this->rot.z += z;

    this->rotVector = DX::XMLoadFloat3(&this->rot);
    void updateWorldMatrix();
}

void Transform::setScaling(const DX::XMVECTOR& scale)
{
    DX::XMStoreFloat3(&this->scale, scale);
    this->scaleVector = scale;
    void updateWorldMatrix();
}

void Transform::setScaling(const DX::XMFLOAT3& scale)
{
    this->scale = scale;
    this->scaleVector = DX::XMLoadFloat3(&this->scale);
    void updateWorldMatrix();
}

void Transform::setScaling(float x, float y, float z)
{
    this->scale = DX::XMFLOAT3(x, y, z);
    this->scaleVector = DX::XMLoadFloat3(&this->scale);
    void updateWorldMatrix();
}

void Transform::adjustScaling(const DX::XMVECTOR& offSet)
{
    this->scaleVector = DX::XMVectorAdd(this->scaleVector, offSet);
    XMStoreFloat3(&this->scale, this->scaleVector);
    void updateWorldMatrix();
}

void Transform::adjustScaling(const DX::XMFLOAT3& offSet)
{
    this->scale.x += offSet.x;
    this->scale.y += offSet.y;
    this->scale.z += offSet.z;

    this->scaleVector = DX::XMLoadFloat3(&this->scale);
    void updateWorldMatrix();
}

void Transform::adjustScaling(float x, float y, float z)
{
    this->scale.x += x;
    this->scale.y += y;
    this->scale.z += z;

    this->scaleVector = DX::XMLoadFloat3(&this->scale);
    void updateWorldMatrix();
}

const DX::XMVECTOR& Transform::getScalingVector() const
{
    return this->scaleVector;
}

const DX::XMFLOAT3& Transform::getScalingFloat3() const
{
    return this->scale;
}

float Transform::getScalingX() const
{
    return this->scale.x;
}

float Transform::getScalingY() const
{
    return this->scale.y;
}

float Transform::getScalingZ() const
{
    return this->scale.z;
}

void Transform::setWorldMatrix(DX::XMMATRIX& matrix)
{
    this->worldMatrix = matrix;
    void updateWorldMatrix();
}

const DX::XMMATRIX& Transform::getWorldMatrix() const
{
    this->worldMatrix;
}

void Transform::setWorldMatrixIdentity()
{
    this->worldMatrix = DX::XMMatrixIdentity();
}
