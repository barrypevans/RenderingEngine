#include "Renderer.h"



GLuint Renderer::gBuffer;
GLuint Renderer::gNormal;
GLuint Renderer::gPosition;
GLuint Renderer::gRoughMetal;
GLuint Renderer::gAlbedo;
GLuint Renderer::rboDepth;
GLuint Renderer::BRDFcaptureFBO;
GLuint Renderer::BRDFcaptureRBO;

Material* Renderer::gwriteMat;
Shader* Renderer::gwriteShader;
Texture*  Renderer::texure_gNormal;
Texture*  Renderer::texure_gPosition;
Texture*  Renderer::texure_gRoughMetal;
Texture*  Renderer::texure_gAlbedo;
Texture* Renderer::IBLSpecIrradianceBRDFTexture;

Material* Renderer::lightPassMat;
Shader*  Renderer::lightPassShader;

  GLuint  Renderer::quadVAO = 0;
  GLuint  Renderer::quadVBO;

int Renderer::Init()
{
	gwriteShader = new Shader("shaders\\deferredgbufferwirte");
	gwriteMat = new Material(gwriteShader);
	lightPassShader = new Shader("shaders\\lightpass_PBR");
	lightPassMat = new Material(lightPassShader);



	//------------Initialize G Buffer-------------------------

	glGenFramebuffers(1, &gBuffer);
	glBindBuffer(GL_FRAMEBUFFER, gBuffer);

	// create position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Window::getResolution().x, Window::getResolution().y, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	
	// create Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Window::getResolution().x, Window::getResolution().y, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// create Roughnes/Metalic color buffer
	glGenTextures(1, &gRoughMetal);
	glBindTexture(GL_TEXTURE_2D, gRoughMetal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window::getResolution().x, Window::getResolution().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gRoughMetal, 0);

	// create Albedo color buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window::getResolution().x, Window::getResolution().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3 , GL_TEXTURE_2D, gAlbedo, 0);

	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3 };

	glDrawBuffers(4, attachments);
	
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Window::getResolution().x, Window::getResolution().y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	//------------Initialize G Buffer-------------------------

	//set up textures for render quad material
	texure_gAlbedo = new Texture(gAlbedo);
	texure_gRoughMetal = new Texture(gRoughMetal);
	texure_gPosition = new Texture(gPosition);
	texure_gNormal = new Texture(gNormal);


	Calculate_IBLSpecIrradianceBRDFTexture();

	return 0;
}

void Renderer::Draw() {
	


	//to depth test
	glEnable(GL_DEPTH_TEST);
	
	// cull backfaces
	glCullFace(GL_BACK);

	//build G Buffer
	GeometryPass();

	//draw the skybox
	if (NULL != Camera::activeCamera->skyBox || NULL != Camera::activeCamera->skyBoxHDR) {
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		Camera::activeCamera->drawSkyBox();
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}

	//Do Shading based on G Buffer Input
	ShadingPass();


	
	//this commented out bit is the old drawing code
	//this will turn into a forward pass
	//glClearColor(0, 49.0f / 256.0f, 77.0f / 256.0f, 121.0f / 256.0f);
	//for (Mesh* m : AllMeshs)
	//	m->Draw();

}

void Renderer::GeometryPass()
{
	//clear black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//bindframebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//Bind all of the color attachments
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	glBindTexture(GL_TEXTURE_2D, gRoughMetal);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gRoughMetal, 0);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gAlbedo, 0);

	//bind the depth attachment
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	//specify locations
	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	//drawout gbuffer
	for (Mesh* m : Scene::activeScene->getAllComponentsofType<Mesh>())
		m->usingShaderDraw(gwriteShader);

	//unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Copy depth component to the frameBuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
	glBlitFramebuffer(
		0, 0, Window::getResolution().x, Window::getResolution().y, 0, 0, Window::getResolution().x, Window::getResolution().y, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::ShadingPass()
{

	//clear to camera clear color
	glClearColor(Camera::activeCamera->clearColor.x, Camera::activeCamera->clearColor.y, Camera::activeCamera->clearColor.z, Camera::activeCamera->clearColor.w);
	if (NULL == Camera::activeCamera->skyBox && NULL == Camera::activeCamera->skyBoxHDR)
		glClear(GL_COLOR_BUFFER_BIT );

	// no more depth test, full screen quad
	glDisable(GL_DEPTH_TEST);
	//dont write to depth buffer
	glDepthMask(GL_FALSE);

	lightPassMat->SetTexture("gbuffer.gPosition", texure_gPosition);
	lightPassMat->SetTexture("gbuffer.gNormal", texure_gNormal);
	lightPassMat->SetTexture("gbuffer.gRoughMetal", texure_gRoughMetal);
	lightPassMat->SetTexture("gbuffer.gAlbedo", texure_gAlbedo);
	lightPassMat->SetTexture("IBLSpecIrradianceBRDF", IBLSpecIrradianceBRDFTexture);

	if(NULL!= Camera::activeCamera->skyBoxHDR)
		lightPassMat->SetCubeMap("environmentMap", Camera::activeCamera->skyBoxHDR);
	else if (NULL != Camera::activeCamera->skyBox)
		lightPassMat->SetCubeMap("environmentMap", Camera::activeCamera->skyBox);

	//Draw FSQ
	RenderQuad(lightPassMat);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::CleanUp()
{
	delete(gwriteMat);
	delete(gwriteShader);
}



// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.

void Renderer::RenderQuad(Material* mat)
{

	//Push light Uniforms
	std::vector<Light*> lights = Scene::activeScene->getAllComponentsofType<Light>();
	int lightCount = lights.size() < 33 ? lights.size() : 32;
	for (int i = 0; i < lightCount; i++) {
		mat->SetVector("lights[" + std::to_string(i) + "].position", lights[i]->entity->transform.position);
		mat->SetVector("lights[" + std::to_string(i) + "].direction",  Vector4(lights[i]->entity->transform.getForward().x, lights[i]->entity->transform.getForward().y, lights[i]->entity->transform.getForward().z, lights[i]->type));
		mat->SetVector("lights[" + std::to_string(i) + "].color", lights[i]->color);

	}
	mat->SetVector("CameraPosition", Camera::activeCamera->entity->transform.position);

	mat->Bind();

	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	mat->Bind();
}


void Renderer::RenderQuad()
{


	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

}



void Renderer::Calculate_IBLSpecIrradianceBRDFTexture()
{



	glGenFramebuffers(1, &BRDFcaptureFBO);
	glGenRenderbuffers(1, &BRDFcaptureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, BRDFcaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, BRDFcaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, BRDFcaptureRBO);


	Shader*  IBLSpecIrradianceBRDFShader = new Shader("shaders/specIrradianceBRDFPreprocessor");
	IBLSpecIrradianceBRDFShader->Init();
	GLuint brdfLUTTexture;
	glGenTextures(1, &brdfLUTTexture);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, BRDFcaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, BRDFcaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	IBLSpecIrradianceBRDFShader->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//glDeleteFramebuffers(1, &captureFBO);
	//glDeleteRenderbuffers(1, &captureRBO);
	glViewport(0, 0, Window::getResolution().x, Window::getResolution().y);
	IBLSpecIrradianceBRDFShader->UnBind();
	delete IBLSpecIrradianceBRDFShader;
	IBLSpecIrradianceBRDFTexture = new Texture(brdfLUTTexture);
}
