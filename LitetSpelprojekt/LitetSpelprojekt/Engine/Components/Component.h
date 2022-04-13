#pragma once

class GameObject;
class Transform;

class Component
{
private:
	GameObject& object;
	Transform* transform;
public:
	Component(GameObject& object);
	virtual ~Component();

	inline GameObject& getObject() { return this->object; };
	inline Transform* getTransform() { return this->transform; };
};