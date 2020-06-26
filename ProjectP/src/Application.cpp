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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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
	/* Vertex Positions */
	float m_vertices[4 * 4] =
	{
		//Position     //Texture Coords
	   -0.5f, -0.5f,   0.0f, 0.0f, // 0 - Bottom Left
	    0.5f, -0.5f,   1.0f, 0.0f, // 1 - Right Side
	    0.5f,  0.5f,   1.0f, 1.0f, // 2 - Top Right
	   -0.5f,  0.5f,   0.0f, 1.0f  // 3 - Left Side
	};

	unsigned int m_indices[2 * 3] =
	{
		0, 1, 2, //First Triangle
		2, 3, 0  //Second Triangle
	};

	VertexArray VA;
	/* VERTEX BUFFER BINDING */
	/* Creates a Vertex Buffer and specifies the size */
	VertexBuffer VB(m_vertices, 4 * 4 * sizeof(float));

	VertexBufferLayout m_Layout;
	/* Specifies what data will go into the Vertex Array */
	m_Layout.Push<float>(2);
	m_Layout.Push<float>(2);
	/* Adds the Vertex Buffer to the Vertex Array */
	VA.AddBuffer(VB, m_Layout);

	/* INDEX BUFFER BINDING */
	IndexBuffer IB(m_indices, 6);

	/* Pass in the Shader Files */
	Shader m_shader("Shaders/ShaderVertTest.vert", "Shaders/ShaderFragTest.frag");

	Texture m_texture("Textures/Future City.png");

	/* Binds the Texture */
	/* Since no argument was put in, it is automatically 0 */
	m_texture.Bind();
	/* Sets the Uniform of Texture at Slot 0 */
	/* Must Match what slot the Texture was bound to */
	m_shader.SetInt("u_Texture", 0);

	VA.UnBind();
	VB.Unbind();
	IB.Unbind();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		/* Process Inputs */
		ProcessInput(m_window);

		/* Renders the background colour */
		glClearColor(0.2, 0.3f, 0.3f, 1.0f);

		/* When we call glClear, the entire colour buffer will be filled with the colour as configured by glClearColor */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Uses Shader Program/Binds the Shader */
		m_shader.UseProgram();

		/* Binds the Vertex Array Buffer (Which also binds the Vertex Buffer Object */
		VA.Bind();
		/* Binds our Index Buffer */
		IB.Bind();

		/* Draws the Shape we defined with our Buffers */
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