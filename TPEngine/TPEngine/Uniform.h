#pragma once
#include "3dMath.h"
#include <glm\glm.hpp>
class Uniform {
public:

	
	enum UType {
		Matrix,
		MatrixGLM,
		Vec2,
		Vec3,
		Vec4,
		Float
	};
	Uniform(Matrix4x4 m, UType t) { mat = m;  type = t; }
	Uniform(Vector2 v, UType t) { v2 = v; type = t;}
	Uniform(glm::mat4 m, UType t) { glmmat = m; type = t;}
	Uniform(Vector3 v, UType t) { v3 = v; type = t;}
	Uniform(Vector4 v, UType t) { v4 = v; type = t;}
	Uniform(float fl, UType t) { f = fl; type = t;}
	Matrix4x4 mat;
	glm::mat4 glmmat;
	Vector2 v2;
	Vector3 v3;
	Vector4 v4;
	float f;
	UType type;
};