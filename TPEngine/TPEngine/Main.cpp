#include <stdio.h>

#include "Window.h"

#include "Renderer.h"
#include "Matrix4x4.h"
#include "Entity.h"

#include "Scene.h"
#include "Mesh.h"
#include "Camera.h"
#include "OBJLoader.h"

#include "Input.h"
#include "Light.h"
#include "CubeMap.h"
#include "CubeMapHDR.h"

int main() {



	//initialize window
	if (Window::Init() == -1)
		return -1;

	if (Renderer::Init() == -1)
		return -1;


	Scene* scene = new Scene();
	scene->setActive();

	glfwSetKeyCallback(Window::glfwwindow, Input::key_callback);
	glfwSetCursorPosCallback(Window::glfwwindow, Input::mouse_callback);

	// GLFW Options
	glfwSetInputMode(Window::glfwwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	Entity* camara = new Entity;

	camara->addComponent<Camera>();

	Entity* light = new Entity();
	light->addComponent<Light>();
	light->transform.position.x = 20;
	light->transform.position.y= 0;
	light->getComponent<Light>()->color = Vector3(100,100,100);

	Entity* light2 = new Entity();
	light2->addComponent<Light>();
	light2->transform.position.y = 0;
	light2->transform.rotation.x = -10;
	light2->getComponent<Light>()->color = Vector3(7,7,7);
	Entity* testEntity = new Entity();
	Entity* testEntity2 = new Entity();
	Shader* shader = new Shader("shaders\\test");
	
	Material* mat = new Material(shader);
	Material* mat2 = new Material(shader);

	Texture* AlbedoTex = new Texture("Cerberus_A.png");
	Texture* RoughTex = new Texture("Cerberus_R.png");
	Texture* MetallicTex = new Texture("Cerberus_M.png");
	Texture* NormalTex = new Texture("Cerberus_N.png");
	mat->SetTexture("material.DiffuseMap", AlbedoTex);
	mat->SetTexture("material.RoughnessMap", RoughTex);
	mat->SetTexture("material.MetallicMap", MetallicTex);
	mat->SetTexture("material.NormalMap", NormalTex);
	mat->SetFloat("material.MetallicVal", 0);
	mat->SetFloat("material.RoughnessVal", 1);
	mat->SetVector("material.DiffuseVal", Vector3(.3f, .3f, .3f));
	mat->SetVector("material.flag", Vector4(1, 1, 1, 1));


	testEntity->addComponent<Mesh>();
	testEntity->getComponent<Mesh>()->material = mat;
	testEntity->getComponent<Mesh>()->setMeshData( OBJLoader::Load("gun.obj"));

	//testEntity->getComponent<Mesh>()->material->SetTexture("testTex", tex);
	//testEntity->getComponent<Mesh>()->material->SetTexture("testTex2", tex2);

	
	/*testEntity2->addComponent<Mesh>();
	testEntity2->getComponent<Mesh>()->material = mat;
	testEntity2->getComponent<Mesh>()->setMeshData(OBJLoader::Load("plane.obj"));*/
	//testEntity2->getComponent<Mesh>()->material->SetTexture("testTex", tex2);
	//testEntity2->getComponent<Mesh>()->material->SetTexture("testTex2", tex);

	scene->AddEntity(testEntity);
	scene->AddEntity(camara);

	//scene->AddEntity(testEntity2);
	scene->AddEntity(light);
	//scene->AddEntity(light2);

	CubeMap* skybox = new CubeMap("testSkyBox");
	CubeMapHDR* skyboxHDR = new CubeMapHDR("Lobby-Center_2k.hdr",true);
	camara->getComponent<Camera>()->skyBox = skybox;
	camara->getComponent<Camera>()->skyBoxHDR = skyboxHDR;


	while (!glfwWindowShouldClose(Window::glfwwindow)) {
		
		//Update Window
		Window::Update();

		//Update GameLoop
		Scene::activeScene->Update();


		Renderer::Draw();

		Window::swapBuffers();


		Input::Update();

	}

	delete(camara);
	//delete(skyboxHDR);
	//delete(skyboxHDR);
	delete(testEntity);
	glfwTerminate();
	Window::CleanUp();
	Renderer::CleanUp();

	return 0;
}

