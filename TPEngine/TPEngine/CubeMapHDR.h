#pragma once
#include <string>
#include <iostream>
#include <GL\glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"
#include "Shader.h"


class Material;
class CubeMapHDR {
public:
	CubeMapHDR(std::string fileName);
	CubeMapHDR(std::string fileName, bool calculateIrradiance);
	~CubeMapHDR();
	void Bind();
	void BindDiffuseIrradiance();
	void BindSpecIrradiance();
	int size=512;
	bool hasIrradianceMap() {return calculateIrradiance;};
private:
	void create(std::string filename, std::string shaderName, GLuint &handle);
	void caluculateDiffuseIrradiance();
	void caluculateSpecularIrradiance();
	GLuint regularHandle;
	GLuint diffuseIrradianceHandle;
	GLuint specIrradianceHandle;
	GLuint cubeVAO;
	GLuint cubeVBO;
	void renderCube();
	bool calculateIrradiance;
	Shader* cubemapConverterShader;
	Shader* cubemapConvolveShader;
	Shader* cubemapSpecIrradianceShader;
}; 
