#include "Camera.h"
#include "glm\glm.hpp"

Camera* Camera::activeCamera;

GLuint Camera::skyboxVAO = -1, Camera::skyboxVBO = -1;

Material* Camera::skyboxMaterial;

Shader* Camera::skyboxShader;
Material* Camera::skyboxMaterialHDR;

Shader* Camera::skyboxShaderHDR;

GLfloat Camera::skyboxVertices[] = {
       
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
};

float Camera::getAspect() { return  Window::getResolution().x / Window::getResolution().y; }
glm::mat4x4 Camera::getProjection()
{
	Vector2 res = Window::getResolution();
	return  glm::perspective(FOV*DEG_TO_RAD, res.x / res.y ,znear, zfar);
}

glm::mat4x4 Camera::getView()
{
	Vector4 f = entity->transform.getRotation() * Vector4(0, 0, 1,0);
	Vector4 u = entity->transform.getRotation() * Vector4(0, 1, 0, 0);
	glm::vec3 f3 = *((glm::vec3*)&Vector3(f.x, f.y, f.z));
	glm::vec3 u3 = *((glm::vec3*)&Vector3(u.x, u.y, u.z));
	//std::cout << (*(Vector3*)&up).to_string() << std::endl;


	float pitch = entity->transform.rotation.x;
	float yaw = entity->transform.rotation.y-90;
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front = glm::normalize(front);
	forward = *(Vector3*)&front;
	return glm::lookAt(*((glm::vec3*)&entity->transform.position), *((glm::vec3*)&entity->transform.position) +front, glm::vec3(0,1,0));
}

void Camera::drawSkyBox()
{
	//initialize Vert buffer object and vert array object
	if (skyboxVAO == -1) {
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);
		skyboxShader = new Shader("shaders\\skybox");
		skyboxMaterial = new Material(skyboxShader);
		skyboxShaderHDR = new Shader("shaders\\skyboxHDR");
		skyboxMaterialHDR = new Material(skyboxShaderHDR);
	}

	
	if (NULL != skyBoxHDR) {

		skyboxMaterialHDR->SetCubeMap("skybox", skyBoxHDR);
		if (Camera::activeCamera != NULL) {
			skyboxMaterialHDR->SetMatGLM("projection", Camera::activeCamera->getProjection());
			skyboxMaterialHDR->SetMatGLM("view", Camera::activeCamera->getView());
		}
		skyboxMaterialHDR->Bind();
	}
	else {

		skyboxMaterial->SetCubeMap("skybox", skyBox);
		if (Camera::activeCamera != NULL) {
			skyboxMaterial->SetMatGLM("projection", Camera::activeCamera->getProjection());
			skyboxMaterial->SetMatGLM("view", Camera::activeCamera->getView());
		}
		skyboxMaterial->Bind();
	}

	
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	if (NULL != skyBoxHDR)
		skyboxMaterialHDR->UnBind();
	else
		skyboxMaterial->UnBind();
}

