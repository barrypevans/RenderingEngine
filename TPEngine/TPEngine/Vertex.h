#pragma once
#include "3dMath.h"
class Vertex {
public:
	Vertex(Vector3 p, Vector3 n,Vector3 t, Vector2 tex) {
		position = p;
		normal = n;
		tangent = t;
		texcoord = tex;
	}
	Vector3 position;
	Vector3 normal;
	Vector3 tangent;
	Vector2 texcoord;
};