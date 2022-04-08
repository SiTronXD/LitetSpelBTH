#pragma once

class ECS;

class GameObject
{
private:
	ECS& ecs;
	int ID;
public:
	GameObject(ECS& ecs, int ID);
	~GameObject();

	int getID() const;

	template <typename T>
	void addComponent();
	template <typename T>
	T* getComponent();
};
