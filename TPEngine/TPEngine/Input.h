#pragma once
#include "Window.h"
#include <GLFW\\glfw3.h>
#include "3dMath.h"

class Input {
public:
	static bool getKey(int key);

	static bool getKeyUp(int key);

	static void Update();

	static void mouse_callback(GLFWwindow * window, double xpos, double ypos);

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	static double mouseDeltax;
	static double mouseDeltay;
private:
	static bool keys[1023];
	static bool keysup[1023];
	static bool notFirstMouseInput;
	static double lastMousex;
	static double lastMousey;
	static bool acceptMouseInput;
};