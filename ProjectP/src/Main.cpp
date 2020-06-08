#include <iostream>

#include <GLFW/glfw3.h>

int main(void)
{
	GLFWwindow* m_window;

	/* Initialize the Library */
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

	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		/* Render Here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(m_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}