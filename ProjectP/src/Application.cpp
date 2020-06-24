#include "Application.h"

void FramBufferSizeCallBack(GLFWwindow* a_window, int a_width, int a_height);

void ProcessInput(GLFWwindow* a_window);

/* Window Settings */
const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 800;

int Application::StartUp(void)
{

	/* Initializes and Configures GLFW */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	if (!glfwInit())
	{
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ProjectP", NULL, NULL);

	/* Checks if the window was created */
	if (!m_window)
	{
		std::cout << " Failed to Create GLFW Window " << std::endl;
		glfwTerminate();
		return -2;
	}

	/* Make the windows context current */
	glfwMakeContextCurrent(m_window);

	/* Checks if the Window has been resized */
	glfwSetFramebufferSizeCallback(m_window, FramBufferSizeCallBack);

	/* Checks if GLAD was loaded correctly */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
}

int Application::Update()
{
	/* Rectangle Primitives */
	float m_vertices[36] =
	{
		//Position           //Colors             //Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    1.0f, 1.0f,	//Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,	//Bottom Right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,	//Bottom Left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,    0.0f, 1.0f	//Top Left
	};

	unsigned int m_indices[6] =
	{
		0, 1, 3, //First Triangle
		1, 2, 3  //Second Triangle
	};

	/* Pass in the Shader Files */
	Shader m_shader("Shaders/ShaderVertTest.vert", "Shaders/ShaderFragTest.frag");

	/* Draws the Triangle */

	/* Vertex, Element and Array Buffer ID's */
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//* Binds the VAO (Vertex Array Object) From this point onwards, we can bind/configure the - */
	//* Corresponding VBO(s) and attribute pointer(s) and then unbind the VAO for later use */
	glBindVertexArray(VAO);


	//* VERTEX BUFFER BINDING */
	//* Generates a Buffer ID using the glGenBuffers (VBO - Vertex Buffer Object) */
	VertexBuffer VB(m_vertices, 4 * 8 * sizeof(float));

	//* INDEX BUFFER BINDING */
	IndexBuffer IB(m_indices, 6);

	//* Sets the Vertex Attribute Pointers */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//* Color Attribute */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//* Texture Coord Attribute */
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Texture m_texture("Textures/WallPaper2.jpg");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		/* Process Inputs */
		ProcessInput(m_window);

		/* Renders the background colour */
		glClearColor(0.2, 0.3f, 0.3f, 1.0f);

		/* When we call glClear, the entire colour buffer will be filled with the colour as configured by glClearColor */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Binds the Texture */
		m_texture.Bind(0);

		/* Uses Shader Program */
		m_shader.UseProgram();

		IB.Bind();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/* Poll for Events */
		glfwPollEvents();
		/* Swap the front and back buffers */
		glfwSwapBuffers(m_window);
	}

	/* Terminates the Application */
	glfwTerminate();
	return 0;
}

/* Terminates Program */
int Application::Terminate()
{
	m_renderer.Terminate();
	glfwTerminate();
	return 0;
}

/* Resize Window Callback Function*/
void FramBufferSizeCallBack(GLFWwindow* a_window, int a_width, int a_height)
{
	/* Tells OpenGL the size of the rendering window */
	glViewport(0, 0, a_width, a_height);
}

/* Input Callback Function */
void ProcessInput(GLFWwindow* a_window)
{
	/* Closes the Window */
	if (glfwGetKey(a_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(a_window, true);
	}
}