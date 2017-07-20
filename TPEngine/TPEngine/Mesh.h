#pragma once
class Renderer;

#include "Material.h"
#include "Renderer.h"
#include "Component.h"
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Mesh_Data.h"
#include "Light.h"
#include "Scene.h"
#include "3dMath.h"
class Mesh : public	Component 
{

public:

	Mesh(Entity* e);
	~Mesh();
	void Init() override;
	void Draw();
	void usingMaterialDraw(Material * mat);
	void usingShaderDraw(Shader * shader);
	void setMeshData(Mesh_Data* d);




	Material* material;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	Material* errorMaterial;

private:
	
	Mesh_Data* data;

};	

