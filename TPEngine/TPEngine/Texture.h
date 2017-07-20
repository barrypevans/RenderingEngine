#pragma once
#include <string>
//#include <SOIL\SOIL.h>
#include <GL\glew.h>
class Texture {
public:
	Texture(std::string fileName);
	Texture(GLuint h);
	~Texture();
	void Bind();
	void UnBind();
	int width;
	int height;
	GLuint getHandle() { return handle; }
private:
	GLuint handle;
	unsigned char* imagedata;
};