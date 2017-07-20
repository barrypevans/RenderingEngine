#pragma once
#include "Shader.h"
#include "3dMath.h"
#include "Matrix4x4.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gl\glew.h>
#include <string>
#include <vector>
#include "Uniform.h"
#include "Texture.h"
#include "CubeMap.h"
#include "CubeMapHDR.h"
class CubeMapHDR;
class Material {
public:
	Material(Shader* s);
	~Material();
	Shader* shader;

	void Bind();
	void UnBind();
	void SetFloat(std::string location, float val);
	void SetVector(std::string location, Vector4 val);
	void SetVector(std::string location, Vector3 val);
	void SetVector(std::string location, Vector2 val);
	void SetMat(std::string location, Matrix4x4 val);
	void SetMatGLM(std::string location, glm::mat4x4 val);


	void BindFloat(std::string location, float val);
	void BindVector(std::string location, Vector4 val);
	void BindVector(std::string location, Vector3 val);
	void BindVector(std::string location, Vector2 val);
	void BindMat(std::string location, Matrix4x4 val);

	void BindMatGLM(std::string location, glm::mat4x4 val);


	void SetTexture(std::string location, Texture* val);
	void SetCubeMap(std::string location, CubeMap * tex);
	void SetCubeMap(std::string location, CubeMapHDR * tex);
private:
	std::vector<Texture*> activeTextures;
	std::vector<std::string> activeTextureLocations;

	std::vector<CubeMap*> activeCubemap;
	std::vector<std::string> activeCubemapLocations;

	std::vector<CubeMapHDR*> activeCubemapHDR;
	std::vector<std::string> activeCubemapHDRLocations;

	std::vector<Uniform> uniforms;
	std::vector<std::string> uniformLocations;

};