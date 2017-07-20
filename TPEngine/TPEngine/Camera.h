#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include "Component.h"
#include "3dMath.h"
#include "CubeMap.h"
#include "CubeMapHDR.h"
#include "Shader.h"
class CubeMapHDR;
class Camera : public Component{
public:

	Camera(Entity* e):Component(e){
		if (activeCamera == NULL)
			activeCamera = this;
		clearColor= Vector4(0, 49.0f / 256.0f, 77.0f / 256.0f, 121.0f / 256.0f);
	}

	float FOV=45;
	float znear = .01;
	float zfar = 100;
	float getAspect();
	glm::mat4x4 getProjection();
	glm::mat4x4  getView();
	 void drawSkyBox();
	CubeMap* skyBox;
	CubeMapHDR* skyBoxHDR;
	Vector4 clearColor;

	static Camera* activeCamera;
private:
	Vector3 forward;
	static GLfloat skyboxVertices[];
	// Setup skybox VAO
	static GLuint skyboxVAO, skyboxVBO;
	static Shader* skyboxShader;
	static Shader* skyboxShaderHDR;
	static Material* skyboxMaterial;
	static Material* skyboxMaterialHDR;
};