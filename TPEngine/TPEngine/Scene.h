#pragma once
#include <vector>
#include "Entity.h"
class Scene {
public :

	Scene();
	void setActive();
	void AddEntity(Entity* e);
	void RemoveEntity(Entity* e);
	void Update();

	static Scene* activeScene;


	template<class T>
	std::vector<T*> getAllComponentsofType() {
		std::vector<T*> result;
		for (int i = 0; i < EntityList.size(); i++) {
			T* temp;
			if ((temp = EntityList[i]->getComponent<T>()) != NULL)
				result.push_back(temp);
		}
		return result;
	}

private:
	std::vector<Entity*> EntityList;

};