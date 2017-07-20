#include "Window.h"




GLFWwindow* Window::glfwwindow;
int Window::Init()
{
	

	


	if (glfwInit() == false) {

		//Initializtion failed
		std::cerr << "GLFW failed to initilaize"<<std::endl;
		return -1;
	}

	//set 4 frags per pixel
	glfwWindowHint(GLFW_SAMPLES, 4);

	//set gl version to 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//use only opengl core profile
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the window
	glfwwindow = glfwCreateWindow(960, 540, "TP Engine - PBR", NULL, NULL);

	//set the window to be the current context
	glfwMakeContextCurrent(glfwwindow);


	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	

	int width, height;
	glfwGetFramebufferSize(glfwwindow, &width, &height);

	glViewport(0, 0, width, height);

	return 0;
}

void Window::Update()
{
	
	glfwPollEvents();


	
}
void Window::swapBuffers()
{

	glfwSwapBuffers(glfwwindow);

}

void Window::CleanUp()
{
//	delete(glfwwindow);
}

Vector2 Window::getResolution()
{
	int width, height;
	glfwGetFramebufferSize(glfwwindow, &width, &height);
	return Vector2(width, height);
}
