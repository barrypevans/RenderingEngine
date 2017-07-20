#pragma once
#include "Component.h"
#include "Entity.h"
#include "3dMath.h"
class Light : public Component{
public:
	Light(Entity* e) : Component(e) {
		color = Vector3(1, 1, 1);
	}
	Vector3 color;
	enum  LightType
	{
		Directional = 0,
		Point = 1,
		Spot = 2
	};
	LightType type = Directional;

};