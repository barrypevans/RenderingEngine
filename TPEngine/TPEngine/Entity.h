#pragma once

#include <vector>
#include "Component.h"
#include "Transform.h"

class Component;
class EntityManager;
class Entity {
public:
	Entity();
	Entity(std::string n) { name = n; }
	~Entity();
	std::string name = "New Entity";
	Transform transform;

	template<class T>
	 void addComponent() {

		 T* temp = new T(this);
		 ((Component*)temp)->Init();
		 Components.push_back(temp);

	 }
	 
	 template<class T>
	 T* getComponent() {
		 T* result = NULL;
		 for (Component* c : Components)
		 {
			 if (typeid(*c) == typeid(T)) {
				 result = (T*)c;
			 }
		 }
		 return result;

	 }
	
	void Update();
private:
	std::vector<Component*> Components;
	float count = 0;
};


