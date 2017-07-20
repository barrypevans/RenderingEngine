#pragma once
#include <GL/glew.h>
#include <string>
class Shader {
public:
	Shader(const std::string file);
	void Init();
	void Bind();
	void UnBind();
	void Create();
	void Reload();
	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint fragmentShader;
	std::string vertshadersource;
	std::string fragshadersource;
	std::string sourceFileName;
	bool compileSuccess = true;
};