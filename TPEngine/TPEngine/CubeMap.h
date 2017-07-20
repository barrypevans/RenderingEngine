#pragma once
#include <string>

#include <GL\glew.h>
#include <vector>
class CubeMap {
public:
	CubeMap(std::string fileName);
	void Bind();
private:
	GLuint handle;
};