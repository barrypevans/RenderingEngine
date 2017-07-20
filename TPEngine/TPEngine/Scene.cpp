#include "Scene.h"
Scene* Scene::activeScene;
Scene::Scene()
{
	
}

void Scene::setActive()
{
	activeScene = this;
}

void Scene::AddEntity(Entity* e)
{
	EntityList.push_back(e);

}

void Scene::RemoveEntity(Entity* e)
{
	for (int i = 0; i < EntityList.size(); i++) {
		if (e == EntityList[i]) {
 			EntityList.erase(EntityList.begin()+i);
			return;
		}
	}
}

void Scene::Update()
{
	for each (Entity* e in EntityList)
	{
		e->Update();
	}
}
