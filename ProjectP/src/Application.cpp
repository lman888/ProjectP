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

	std::cout << glGetString(GL_VERSION) << std::endl;
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

	/* Maps all our Coords on a 2D plane */
	glm::mat4 m_OrthoProj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

	/* Pass in the Shader Files */
	Shader m_shader("Shaders/ShaderVertTest.vert", "Shaders/ShaderFragTest.frag");

	/* Binds the Shader */
	m_shader.Bind();

	/* Texture Location */
	Texture m_texture("Textures/Future City.png");

	/* Binds the Texture */
	/* Since no argument was put in, it is automatically 0 */
	m_texture.Bind();
	/* Sets the Uniform of Texture at Slot 0 */
	/* Must Match what slot the Texture was bound to */
	m_shader.SetInt("u_Texture", 0);

	m_shader.SetUniformMat4F("u_MVP", m_OrthoProj);

	VA.UnBind();
	VB.UnBind();
	IB.UnBind();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		/* Process Inputs */
		ProcessInput(m_window);

		/* Renders the background colour */
		glClearColor(0.2, 0.3f, 0.3f, 1.0f);

		/* When we call glClear, the entire colour buffer will be filled with the colour as configured by glClearColor */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Binds the Vertex Array Buffer (Which also binds the Vertex Buffer Object */
		VA.Bind();
		/* Binds our Index Buffer */
		IB.Bind();

		/* Draws the Shape we defined with our Buffers */
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		/* Draws Triagnles in WireFrame Mode */
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		/* Sets the Triangles to its default look */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/* Swap the front and back buffers */
		glfwSwapBuffers(m_window);
		/* Poll for Events */
		glfwPollEvents();
	}

	/* Terminates the Application */
	return 0;
}

void Application::Terminate()
{
	glfwTerminate();
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