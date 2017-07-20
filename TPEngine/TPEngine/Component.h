#pragma once

#include "Entity.h"

class Entity;
class Component {
public:
	Component(Entity* e) {
		entity = e;
	}
	virtual void Init() {}
	Entity* entity;

};