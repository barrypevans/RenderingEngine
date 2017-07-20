#include "Mesh.h"
#include "Matrix4x4.h"
#include "Camera.h"

Mesh::Mesh(Entity* e) :Component(e)
{
	errorMaterial = new Material(new Shader("shaders\\error"));
}

Mesh::~Mesh()
{

	
	delete(errorMaterial);

}


void  Mesh::Init()
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

}

void Mesh::Draw() {
	Material* mat;
	bool validMaterial = NULL != material;
	if (validMaterial)
		validMaterial = material->shader->compileSuccess;
	
	mat = validMaterial ? material : errorMaterial;
	
	//Push Transformation Uniforms
	mat->SetMat("model", entity->transform.getTranslation()* (entity->transform.getRotation()*entity->transform.getScale()));
	if (Camera::activeCamera != NULL) {
		mat->SetMatGLM("projection", Camera::activeCamera->getProjection());
		mat->SetMatGLM("view", Camera::activeCamera->getView());
		mat->SetVector("cameraPos", Vector4(Camera::activeCamera->entity->transform.position, 1));
		//std::cout << Camera::activeCamera->getProjection().to_string() << std::endl;
	}


	//Push light Uniforms
	std::vector<Light*> lights = Scene::activeScene->getAllComponentsofType<Light>();
	int lightCount = lights.size() < 33 ? lights.size() : 32;
	for (int i = 0; i < lightCount; i++) {
		mat->SetVector("lights[" + std::to_string(i) + "].direction", lights[i]->entity->transform.getForward());
		mat->SetVector("lights[" + std::to_string(i) + "].color", lights[i]->color);
		mat->SetVector("lights[" + std::to_string(i) + "].position", lights[i]->entity->transform.position);
	}

	mat->Bind();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, data->verticies.size());
	glBindVertexArray(0);

	

	mat->UnBind();
		
}


void Mesh::usingMaterialDraw(Material* mat) {
	
	bool validMaterial = NULL != mat;
	if (validMaterial)
		validMaterial = mat->shader->compileSuccess;

	mat = validMaterial ? mat : errorMaterial;




	mat->SetMat("model", entity->transform.getTranslation()* (entity->transform.getRotation()*entity->transform.getScale()));
	if (Camera::activeCamera != NULL) {
		mat->SetMatGLM("projection", Camera::activeCamera->getProjection());
		mat->SetMatGLM("view", Camera::activeCamera->getView());
		mat->SetVector("cameraPos", Vector4(Camera::activeCamera->entity->transform.position, 1));
		//std::cout << Camera::activeCamera->getProjection().to_string() << std::endl;
	}


	//Push light Uniforms
	std::vector<Light*> lights = Scene::activeScene->getAllComponentsofType<Light>();
	int lightCount = lights.size() < 33 ? lights.size() : 32;
	for (int i = 0; i < lightCount; i++) {
		mat->SetVector("lights[" + std::to_string(i) + "].position", lights[i]->entity->transform.position);
		mat->SetVector("lights[" + std::to_string(i) + "].direction", lights[i]->entity->transform.getForward());
		mat->SetVector("lights[" + std::to_string(i) + "].color", lights[i]->color);
	
	}

	mat->Bind();
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, data->verticies.size());
	glBindVertexArray(0);

	mat->UnBind();

	mat = NULL;
}
void Mesh::usingShaderDraw(Shader* shader) {
	
	Shader* originalShader;

	if (NULL == material)
		return;

	originalShader = material->shader;
	material->shader = shader;





	material->SetMat("model", entity->transform.getTranslation()* (entity->transform.getRotation()*entity->transform.getScale()));
	if (Camera::activeCamera != NULL) {
		material->SetMatGLM("projection", Camera::activeCamera->getProjection());
		material->SetMatGLM("view", Camera::activeCamera->getView());
		material->SetVector("cameraPos", Vector4(Camera::activeCamera->entity->transform.position, 1));
		//std::cout << Camera::activeCamera->getProjection().to_string() << std::endl;
	}

	//Push light Uniforms
	std::vector<Light*> lights = Scene::activeScene->getAllComponentsofType<Light>();
	int lightCount = lights.size() < 33 ? lights.size() : 32;
	for (int i = 0; i < lightCount; i++) {
		material->SetVector("lights[" + std::to_string(i) + "].direction", lights[i]->entity->transform.getForward());
		material->SetVector("lights[" + std::to_string(i) + "].color", lights[i]->color);
		material->SetVector("lights[" + std::to_string(i) + "].position", lights[i]->entity->transform.position);
	}


	material->Bind();

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, data->verticies.size());
	glBindVertexArray(0);

	material->UnBind();

	material->shader = originalShader;
}


void Mesh::setMeshData(Mesh_Data * d)
{

	data = d;


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, data->verticies.size() * sizeof(Vertex), &data->verticies[0], GL_STATIC_DRAW);



	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->indices.size() * sizeof(GLuint), &data->indices[0], GL_STATIC_DRAW);

	//bind Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//bind normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,normal));

	//bind tangents
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));


	//bind texcoord
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));



	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}
