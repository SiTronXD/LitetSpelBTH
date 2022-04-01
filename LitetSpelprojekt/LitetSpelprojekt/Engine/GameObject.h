#pragma once

class ECS;

class GameObject
{
private:
	ECS& ecs;

public:
	GameObject(ECS& ecs);
	~GameObject();

	/*template <typename T>
	void addComponent();*/
};

/*template<typename T>
inline void GameObject::addComponent()
{
	ecs.addActiveComponent<T>(*this);
}*/
