#pragma once
#include "3dMath.h"
#include "glm\glm.hpp"
class Transform {

public:
	Transform() {
		scale.x = 1;
		scale.y = 1;
		scale.z = 1;
		rotation.x = 0;
		rotation.y = 0;
		rotation.z = 0;
	}
	Vector3 position;
	Vector3 scale;
	Vector3 rotation;

	Matrix4x4 getTranslation() {
		Matrix4x4 trans;
		trans.set(0, 0, 1);
		trans.set(1, 1, 1);
		trans.set(2, 2, 1);
		trans.set(3, 3, 1);

		trans.set(3, 0, position.x);
		trans.set(3, 1, position.y);
		trans.set(3, 2, position.z);
		return trans;
	}

 
	Matrix4x4 getScale() {
		Matrix4x4 scalemat;
		scalemat.set(0, 0, scale.x);
		scalemat.set(1, 1, scale.y);
		scalemat.set(2, 2, scale.z);
		scalemat.set(3, 3, 1);

		return scalemat;
	}


	Matrix4x4 getRotation() {
		Matrix4x4 rotx;
		rotx.set(0, 0, 1);
		rotx.set(1, 1, cos(rotation.x*DEG_TO_RAD));
		rotx.set(2, 1, -sin(rotation.x*DEG_TO_RAD));
		rotx.set(1, 2, sin(rotation.x*DEG_TO_RAD));
		rotx.set(2, 2, -cos(rotation.x*DEG_TO_RAD));
		rotx.set(3, 3, 1);

		Matrix4x4 roty;
		roty.set(0, 0, cos(rotation.y*DEG_TO_RAD));
		roty.set(2, 0, sin(rotation.y*DEG_TO_RAD));
		roty.set(1, 1, 1);
		roty.set(0, 2, -sin(rotation.y*DEG_TO_RAD));
		roty.set(2, 2, cos(rotation.y*DEG_TO_RAD));
		roty.set(3, 3, 1);

		Matrix4x4 rotz;
		rotz.set(0, 0, cos(rotation.z*DEG_TO_RAD));
		rotz.set(1, 0, -sin(rotation.z*DEG_TO_RAD));
		rotz.set(1, 1, cos(rotation.z*DEG_TO_RAD));
		rotz.set(0, 1, sin(rotation.z*DEG_TO_RAD));
		rotz.set(2, 2, 1);
		rotz.set(3, 3, 1);
		
		return rotz*(roty*rotx);
	}
	Vector3 getForward() {
		float pitch =rotation.x;
		float yaw = rotation.y - 90;
		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		front = glm::normalize(front);
		return *(Vector3*)&front;
	}

	Vector3 getUp() {
		return Vector3(0, 1, 0);
	}

	Vector3 getRight() {
		return *(Vector3*)&glm::normalize(glm::cross(*(glm::vec3*)&getForward(), glm::vec3(0, 1, 0)));
	}
private:
	
};