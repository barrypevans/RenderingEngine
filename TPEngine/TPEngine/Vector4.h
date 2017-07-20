#pragma once
#include "Vector3.h"
class Vector4 {
public:
	Vector4(float in_x, float in_y, float in_z, float in_w) 
	{ 
	x = in_x; 
	y = in_y;
	z = in_z;
	w = in_w;
	}
	Vector4()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	Vector4(Vector3 v, float in_w)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = in_w;
	}

	Vector3 xyz() {
		return Vector3(x, y, z);
	}

	float x;
	float y;
	float z;
	float w;

	

	float& operator[](std::size_t index){
		switch (index)
		{

		case 0: return x;

		case 1:return y;

		case 2:return z;

		case 3:return w;

		}
	}
	std::string to_string() {
		
		return "[ " + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(w) + "]";
	}
};