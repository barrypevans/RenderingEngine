#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string fileName)
{
	std::string txt;
	std::ifstream file(fileName+".vs");
	if (file.is_open())
		while (file.good())
		{
			getline(file, txt);
			vertshadersource += txt;
			vertshadersource += "\n";
		}
	
	std::string txt2;
	std::ifstream file2(fileName + ".fs");
	if (file2.is_open())
		while (file2.good())
		{
			getline(file2, txt2);
			fragshadersource += txt2;
			fragshadersource += "\n";
		}
	
	sourceFileName = fileName;
}

void Shader::Init()
{
	Create();
}




void Shader::Create() {
	
	compileSuccess = true;

	const char* vertshadersourcec_str = vertshadersource.c_str();
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertshadersourcec_str, NULL);
	glCompileShader(vertexShader);


	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << sourceFileName << "\n" << infoLog << std::endl;
		compileSuccess = false;
	}
	else {
		//std::cout << "vertex shader compiled" << std::endl;
		
	}


	const char* fragshadersourcec_str = fragshadersource.c_str();
	
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragshadersourcec_str, NULL);
	glCompileShader(fragmentShader);



	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FFAGMENT::COMPILATION_FAILED" << sourceFileName << "\n" << infoLog << std::endl;
		compileSuccess = false;
	}
	else {
		//std::cout << "fragment shader compiled" << std::endl;
	}
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED - "<< sourceFileName <<"\n" << infoLog << std::endl;
		compileSuccess = false;
	}
	else {
		//std::cout << "linking success!" << std::endl;
	
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Reload() {
	vertshadersource.clear();
	fragshadersource.clear();
	std::string txt;
	std::ifstream file(sourceFileName + ".vs");
	if (file.is_open())
		while (file.good())
		{
			getline(file, txt);
			vertshadersource += txt;
			vertshadersource += "\n";
		}

	std::string txt2;
	std::ifstream file2(sourceFileName + ".fs");
	if (file2.is_open())
		while (file2.good())
		{
			getline(file2, txt2);
			fragshadersource += txt2;
			fragshadersource += "\n";
		}

	glDeleteProgram(shaderProgram);
	
	Create();
}

void Shader::Bind()
{
	glUseProgram(shaderProgram);
	
}
void Shader::UnBind()
{
	glUseProgram(0);

}
