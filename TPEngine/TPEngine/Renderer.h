#pragma once

#include <iostream>

#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Mesh.h"
#include "Camera.h"
#include "Window.h"
#include "Scene.h"

class Mesh;
class Renderer {


public:
	static int Init();
	static void Draw();
	static void CleanUp();
	static void GeometryPass();
	static void ShadingPass();
	static void RenderQuad(Material* mat);

	static void RenderQuad();

private:

	static GLuint gBuffer;
	static GLuint gNormal;
	static GLuint gPosition;
	static GLuint gRoughMetal;
	static GLuint gAlbedo;

	static GLuint rboDepth;

	static Texture* texure_gNormal;
	static Texture* texure_gPosition;
	static Texture* texure_gRoughMetal;
	static Texture* texure_gAlbedo;

	static Material* gwriteMat;
	static Shader* gwriteShader;
	static Material* lightPassMat;
	static Shader* lightPassShader;

	static GLuint quadVAO;
	static GLuint quadVBO;

	static GLuint BRDFcaptureFBO;
	static GLuint BRDFcaptureRBO;

	static Texture* IBLSpecIrradianceBRDFTexture;
	
	static void Calculate_IBLSpecIrradianceBRDFTexture();
}; 
