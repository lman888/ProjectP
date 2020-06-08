#include "Application.h"

int Application::StartUp()
{
	if (!glfwInit())
	{
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	m_window = glfwCreateWindow(640, 480, "ProjectP", NULL, NULL);

	if (!m_window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the windows context current */
	glfwMakeContextCurrent(m_window);
}

void Application::Update()
{
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		/* Render Here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap the front and back buffers */
		glfwSwapBuffers(m_window);

		/* Poll for Events */
		glfwPollEvents();
	}

	glfwTerminate();
}

void Application::Terminate()
{
	glfwTerminate();
}
