#include "Application.h"

void FramBufferSizeCallBack(GLFWwindow* a_window, int a_width, int a_height);

void ProcessInput(GLFWwindow* a_window);

int Application::StartUp(void)
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	if (!glfwInit())
	{
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	m_window = glfwCreateWindow(640, 480, "ProjectP", NULL, NULL);

	/* Checks if the window was created */
	if (!m_window)
	{
		std::cout << " Failed to Create GLFW Window " << std::endl;
		glfwTerminate();
		return -2;
	}

	/* Make the windows context current */
	glfwMakeContextCurrent(m_window);

	/* Checks if GLAD was loaded correctly */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/* Tells OpenGL the size of the rendering window */
	glViewport(0, 0, 640, 480);

}

int Application::Update()
{
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		/* Process Inputs */
		ProcessInput(m_window);

		/* Renders the background colour */
		glClearColor(0.2, 0.3f, 0.3f, 1.0f);

		/* When we call glClear, the entire colour buffer will be filled with the colour as configured by glClearColor */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Checks if the Window has been resized */
		glfwSetFramebufferSizeCallback(m_window, FramBufferSizeCallBack);

		/* Poll for Events */
		glfwPollEvents();

		/* Swap the front and back buffers */
		glfwSwapBuffers(m_window);
	}

	glfwTerminate();
	return 0;
}

int Application::Terminate()
{
	glfwTerminate();
	return 0;
}

/* Resize Window Callback Function*/
void FramBufferSizeCallBack(GLFWwindow* a_window, int a_width, int a_height)
{
	glViewport(0, 0, a_width, a_height);
}

void ProcessInput(GLFWwindow* a_window)
{
	/* Closes the Window */
	if (glfwGetKey(a_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(a_window, true);
	}
}