#pragma once
#include <string>
#include <iostream>  
#include "Vector4.h"
class Matrix4x4 {
public:

	Matrix4x4(){
		for (int i = 0; i < 16; i++)
			data[i] = 0;
	}

	float data [16];

	float get(int i_x, int i_y) {
		return data[i_x+4*i_y];
	}
	void set(int i_x, int i_y,float val) {
		 data[i_x + 4 * i_y]=val;
	}

	Matrix4x4 operator *(Matrix4x4 other_mat) {
		Matrix4x4 result;
		
		
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					float temp = 0;
					temp += get(0, j) * other_mat.get(i, 0);
					temp += get(1, j) * other_mat.get(i, 1);
					temp += get(2, j) * other_mat.get(i, 2);
					temp += get(3, j) * other_mat.get(i, 3);
					result.set(i, j, temp);
				}
				
			}
		
		return result;
	}

	Vector4 operator *(Vector4 vec) {
		Vector4 result;


			for (int j = 0; j < 4; j++) {
				float temp = 0;
				temp += get(0, j) * vec[0];
				temp += get(1, j) * vec[1];
				temp += get(2, j) * vec[2];
				temp += get(3, j) * vec[3];
				result[j] = temp;
			}

		

		return result;
	}



	std::string to_string() {
		std::string result = "{\n";
		for (int i = 0; i < 4; i++) {
			result += "[";
			for (int j = 0; j < 4; j++) {
				result += std::to_string(get(i, j));
			}
			result += "],\n";
		}
		result += "}";
		return result;
	}

	
};
