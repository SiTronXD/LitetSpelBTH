#pragma once

#include "ECS.h"

struct RaycastInfo;

class GameObject
{
private:
	ECS& ecs;
	int ID;

	std::string name;
	ObjectTag tag;
public:
	GameObject(ECS& ecs, int ID, std::string name, ObjectTag tag);
	~GameObject();

	// Cast from own position and direction
	RaycastInfo raycast(float maxDist);
	// Define own ray position and direction
	RaycastInfo raycast(rp3d::Ray ray);

	void playSound(std::string sound);

	template <typename T>
	T* addComponent();
	template <typename T>
	bool removeComponent();
	template <typename T>
	T* getComponent();

	inline const int getID() { return this->ID; };
	inline const std::string getName() { return this->name; }
	inline const ObjectTag getTag() { return this->tag; }
	inline std::vector<Component*> getAllComponents() { return this->ecs.getAllComponents(this->ID); };
};

template<typename T>
inline T* GameObject::addComponent()
{
	return this->ecs.addComponent<T>(this->ID);
}

template<typename T>
inline bool GameObject::removeComponent()
{
	return this->ecs.removeComponent<T>(this->ID);
}

template<typename T>
inline T* GameObject::getComponent()
{
	return this->ecs.getComponent<T>(this->ID);
}
