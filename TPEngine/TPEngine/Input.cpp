#include "Input.h"

bool Input::keys[1023];

bool Input::keysup[1023];
bool Input::notFirstMouseInput;
double Input::lastMousex;
double Input::lastMousey;
double Input::mouseDeltax;
double Input::mouseDeltay;
bool Input::acceptMouseInput;
bool Input::getKey(int key)
{
	return keys[key];
}

bool Input::getKeyUp(int key)
{
	return keysup[key];
}

void Input::Update() {
	for(int i=0; i< 1023;i++)
	keysup[i] = false;
}

void Input::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetInputMode(Window::glfwwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		acceptMouseInput = false;
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		glfwSetInputMode(Window::glfwwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		acceptMouseInput = true;
		notFirstMouseInput = false;
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
			keysup[key] = true;
		}
	}
}
void Input::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (!notFirstMouseInput)
	{
		acceptMouseInput = true;
		lastMousex = xpos;
		lastMousey = ypos;
		notFirstMouseInput = true;
		return;
	}

	if (!acceptMouseInput) {
		mouseDeltax = mouseDeltay = 0;
		return;
	}

	mouseDeltax = xpos - lastMousex;
	mouseDeltay = ypos - lastMousey;
	//mouseDeltax = abs(mouseDeltax) < .01 ? 0 : mouseDeltax;
	//mouseDeltay = abs(mouseDeltay) < .01 ? 0 : mouseDeltay;
	lastMousex = xpos;
	lastMousey = ypos;
}
