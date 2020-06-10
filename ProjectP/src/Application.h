#pragma once
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

class Application
{

public:

	int StartUp(void);

	int Update();

	int Terminate();

protected:


private:

	GLFWwindow* m_window = NULL;

};