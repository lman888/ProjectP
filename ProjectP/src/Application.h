#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

class Application
{

public:

	bool StartUp();

	void Update();

	void Terminate();

protected:


private:

	GLFWwindow* m_window = NULL;

};