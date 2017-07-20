#pragma once
#include <string>

class Vector2 {
public:
	Vector2(float in_x, float in_y) 
	{
		x = in_x;
		y = in_y;
	}

	Vector2() 
	{

	}

	float x = 0; float y = 0;
	float& operator[](std::size_t index) {
		switch (index)
		{

		case 0: return x;

		case 1:return y;


		}
	}
	std::string to_string() {

		return "[ " + std::to_string(x) + "," + std::to_string(y)  + "]";
	}

	Vector2 operator+(Vector2 v) {
		return Vector2(x + v.x, y + v.y);
	}
	Vector2 operator-(Vector2 v) {
		return Vector2(x - v.x, y - v.y);
	}
	Vector2 operator*(float f) {
		return Vector2(x*f, y*f);
	}

	Vector2 operator-=(Vector2 v) {
		return Vector2(x - v.x, y - v.y);
	}
	Vector2 operator+=(Vector2 v) {
		return Vector2(x + v.x, y + v.y);
	}
};