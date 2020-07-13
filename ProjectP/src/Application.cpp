#include "Application.h"

void FramBufferSizeCallBack(GLFWwindow* a_window, int a_width, int a_height);

void ProcessInput(GLFWwindow* a_window);

/* Window Settings */
const unsigned int SCR_HEIGHT = 540;
const unsigned int SCR_WIDTH = 960;

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

	/* Version of OpenGL */
	std::cout << glGetString(GL_VERSION) << std::endl;
}

int Application::Update()
{
	/* Vertex Positions */
	float m_vertices[4 * 4] =
	{
		//Position       //Texture Coords
	    100.0f, 100.0f,  0.0f, 0.0f, // 0 - Bottom Left
	    200.0f, 100.0f,  1.0f, 0.0f, // 1 - Right Side
	    200.0f, 200.0f,  1.0f, 1.0f, // 2 - Top Right
	    100.0f, 200.0f,  0.0f, 1.0f  // 3 - Left Side
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
	glm::mat4 m_view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
	/* Model Matrix */
	glm::mat4 m_model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

	/* Model View Projection Calculation */
	/* (In OpenGL its Projection View Model) */
	glm::mat4 m_mvp = m_proj * m_view * m_model;


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
	m_shader.SetUniform1i("u_Texture", 0);

	/* Sets the Objects Projection */
	m_shader.SetUniformMat4F("u_MVP", m_mvp);

	VA.UnBind();
	VB.UnBind();
	IB.UnBind();

	/* IMGUI Setup */
	//ImGui::CreateContext();
	///* Setup Dear ImGui Style */
	//ImGui::StyleColorsDark();
	///* Setup Platform/Render bindings */
	//ImGui_ImplGlfw_InitForOpenGL(m_window, true);

	/* ImGui States */
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Renderer m_renderer;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(m_window))
	{
		/* Process Inputs */
		ProcessInput(m_window);

		/* Renders the background colour */
		glClearColor(0.2, 0.3f, 0.3f, 1.0f);

		/* When we call glClear, the entire colour buffer will be filled with the colour as configured by glClearColor */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Starts the Dear ImGui Frame */
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();

		m_renderer.Draw(VA, IB, m_shader);

		/* Binds the Vertex Array Buffer (Which also binds the Vertex Buffer Object */
		VA.Bind();
		/* Binds our Index Buffer */
		IB.Bind();

		/* ImGui Window Render */
		//if (show_demo_window)
		//	ImGui::ShowDemoWindow(&show_demo_window);
		//{
		//	static float f = 0.0f;
		//	static int counter = 0;

		//	/* Create a window and append into it */
		//	ImGui::Begin("Project P");
		//	/* Display some text */
		//	ImGui::Text("Fun Big Project");
		//	/* Edit bools storing out window open/close state */
		//	ImGui::Checkbox("Demo Window", &show_demo_window);
		//	ImGui::Checkbox("Another Window", &show_another_window);

		//	/* Edit 1 float using a slider from 0.0f to 1.0f */
		//	ImGui::SliderFloat("Float", &f, 0.0f, 1.0f);
		//	/* Edit 3 floats representing a color */
		//	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);

		//	/* Buttons return true when clicked (most widgets return true when edited/activated) */
		//	if (ImGui::Button("Button"))
		//	{
		//		counter++;
		//	}
		//	ImGui::SameLine();
		//	ImGui::Text("Counter = %d", counter);


		//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//	ImGui::End();
		//}

		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap the front and back buffers */
		glfwSwapBuffers(m_window);
		/* Poll for Events */
		glfwPollEvents();
	}

	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();

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