#pragma once
#include <GLFW/glfw3.h>


class Application
{

public:

	int StartUp();

	void Update();

	void Terminate();

protected:

private:
	
	GLFWwindow* m_window;

};