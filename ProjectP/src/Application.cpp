#include "Application.h"


void FramBufferSizeCallBack(GLFWwindow* a_window, int a_width, int a_height);

void ProcessInput(GLFWwindow* a_window);

/* Window Settings */
const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 1000;

/* Time between current frame and last frame */
float m_deltaTime = 0.0f;

/* Time of last frame */
float m_lastFrame = 0.0f;

int Application::StartUp(void)
{

	/* Initializes and Configures GLFW */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

	/* Version of OpenGL */
	std::cout << glGetString(GL_VERSION) << std::endl;

	const GLubyte* m_renderer = glGetString(GL_RENDERER);
	const GLubyte* m_vendor = glGetString(GL_VENDOR);
	const GLubyte* m_version = glGetString(GL_VERSION);
	const GLubyte* m_glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint m_major, m_minor;
	glGetIntegerv(GL_MAJOR_VERSION, &m_major);
	glGetIntegerv(GL_MINOR_VERSION, &m_minor);

	printf("GL Vendor               :%s\n", m_vendor);
	printf("GL Renderer             :%s\n", m_renderer);
	printf("GL Version (string)     :%s\n", m_version);
	printf("GL Version (integer)    :%d.%d\n", m_major, m_minor);
	printf("GLSL Version            :%s\n", m_glslVersion);
}

int Application::Update()
{
	/* Vertex Positions */
	float m_vertices[4 * 4] =
	{
		//Position       //Texture Coords
	   -50.0f, -50.0f,  0.0f, 0.0f, // 0 - Bottom Left
		50.0f, -50.0f,  1.0f, 0.0f, // 1 - Right Side
		50.0f,  50.0f,  1.0f, 1.0f, // 2 - Top Right
	   -50.0f,  50.0f,  0.0f, 1.0f  // 3 - Left Side
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

	/* (Orthographic Projection) Maps all our Coords on a 2D plane (Left, Right, Bottom, Far, Near) */
	glm::mat4 m_proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	/* View Matrix */
	//glm::mat4 m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	/* Pass in the Shader Files */
	Shader m_shader("Shaders/ShaderVertTest.vert", "Shaders/ShaderFragTest.frag");

	/* Binds the Shader */
	m_shader.Bind();

	/* Texture Location */
	Texture m_texture("Textures/Future City.png");
	Texture m_textuerTwo("Textures/Trying.png");

	/* Binds the Texture */
	/* If no argument was put in, it is automatically 0 */
	/* Assigns Texture to Slot 0 */
	m_texture.Bind(0);
	/* Binds to Texture Slot 1 */
	m_textuerTwo.Bind(1);

	/* Sets the Uniform of Texture at Slot 0 */
	/* Must Match what slot the Texture was bound to */
	//m_shader.SetUniform1i("u_Texture", 1);

	VA.UnBind();
	VB.UnBind();
	IB.UnBind();

	/* IMGUI Setup */
	const char* glsl_version = "#version 450";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	/* Setup Dear ImGui Style */
	ImGui::StyleColorsDark();
	/* Setup Platform/Render bindings */
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	/* Objects Initial Position */
	glm::vec3 m_translationA(200, 200, 0);
	glm::vec3 m_translationB(500, 0200, 0);

	Renderer m_renderer;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		/* Process Inputs */
		ProcessInput(m_window);

		m_renderer.Clear();

		/* Starts the Dear ImGui Frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float m_currentFrame = glfwGetTime();
		m_deltaTime = m_currentFrame - m_lastFrame;
		m_lastFrame = m_currentFrame;

		CameraInputs(m_window);

		/* Renders Two objects (Exact same object) */
		{
			/* Model Matrix */
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationA);
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mvp = m_proj * m_view * m_model;
			m_shader.SetUniform1i("u_Texture", 0);
			m_shader.SetUniformMat4F("u_MVP", m_mvp);
			m_renderer.Draw(VA, IB, m_shader);
		}
		{
			/* Model Matrix */
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationB);
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mvp = m_proj * m_view * m_model;
			m_shader.SetUniform1i("u_Texture", 1);
			m_shader.SetUniformMat4F("u_MVP", m_mvp);
			m_renderer.Draw(VA, IB, m_shader);
		}

		/* ImGui Window Render */
		{
			/* Edit 1 float using a slider from 0.0f to 960.0f */
			ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f);
			ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.0f, 960.0f);
			ImGui::SliderFloat("Camera Speed", &m_cameraSpeedValue, 0.0f, 500.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap the front and back buffers */
		glfwSwapBuffers(m_window);
		/* Poll for Events */
		glfwPollEvents();
	}

	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	/* Terminates the Application */
	return 0;
}

void Application::Terminate()
{
	glfwTerminate();
}

void Application::CameraInputs(GLFWwindow* a_window)
{
	m_view = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);

	m_cameraSpeed = m_cameraSpeedValue * m_deltaTime;
	if (glfwGetKey(a_window, GLFW_KEY_W) == GLFW_PRESS)
		m_cameraPosition += m_cameraSpeed * m_cameraFront;
	if (glfwGetKey(a_window, GLFW_KEY_S) == GLFW_PRESS)
		m_cameraPosition -= m_cameraSpeed * m_cameraFront;
	if (glfwGetKey(a_window, GLFW_KEY_A) == GLFW_PRESS)
		m_cameraPosition -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_cameraSpeed;
	if (glfwGetKey(a_window, GLFW_KEY_D) == GLFW_PRESS)
		m_cameraPosition += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_cameraSpeed;
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