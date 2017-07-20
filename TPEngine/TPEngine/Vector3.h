#pragma once
#include <string>

class Vector3{
public:
	Vector3(float in_x, float in_y, float in_z)
	{
		x = in_x;
		y = in_y;
		z = in_z;
	}

	Vector3() 
	{
	
	}
	float x;
	float y;
	float z;
	float& operator[](std::size_t index) {
		switch (index)
		{

		case 0: return x;

		case 1:return y;

		case 2:return z;

		}
	}

	

	std::string to_string() {

		return "[ " + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) +  "]";
	}

	static float dot(Vector3 v1, Vector3 v2) {
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}


	Vector3 operator+(Vector3 v) {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	Vector3 operator-(Vector3 v) {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3 operator*(float f) {
		return Vector3(x*f, y*f, z *f);
	}

	Vector3 operator-=(Vector3 v) {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3 operator+=(Vector3 v) {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	void Normalize() {
		float length = sqrt(x*x + y*y + z*z);
		x /= length;
		y /= length;
		z /= length;
	}
};