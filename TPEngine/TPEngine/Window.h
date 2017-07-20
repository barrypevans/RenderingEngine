#pragma once
#include "Renderer.h"
#include "3dMath.h"
#include <iostream>
#include <gl\glew.h>
#include <GLFW\glfw3.h>

class Window {
	public :
		static int Init();
		static void Update();
		static void swapBuffers();
		static void CleanUp();
		static Vector2 getResolution();
		static GLFWwindow* glfwwindow;
		
};