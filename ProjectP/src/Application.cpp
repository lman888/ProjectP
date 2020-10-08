#include "Application.h"

void FramBufferSizeCallBack(GLFWwindow* a_window, int a_width, int a_height);
void ProcessInput(GLFWwindow* a_window);
void MouseCallback(GLFWwindow* a_window, double a_xpos, double a_ypos);
void MouseScrollCallback(GLFWwindow* a_window, double a_XoffSet, double a_yOffSet);
void DebugCallBack(GLenum a_source, GLenum a_type, GLuint a_ID,
	GLenum a_severity, GLsizei a_length, const GLchar* a_message,
	const void* a_param);

/* Calls the GLFW Window */
GLFWwindow* m_window = NULL;

/* Window Settings */
const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 1000;

/* Camera Variables */
Camera m_camera;
float m_cameraSpeedValue;
bool firstMouse = true;
float m_sensitivity = 0.01f;

/* Initialize Mouse Pointer to be roughly Centre of the screen */
float m_lastX = 500, m_lastY = 300;

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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	

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

	/* Callback Functions */
	/* Checks if the Window has been resized */
	glfwSetFramebufferSizeCallback(m_window, FramBufferSizeCallBack);
	/* Calculates the Mouse's current position on the screen */
	glfwSetCursorPosCallback(m_window, MouseCallback);
	//glfwSetScrollCallback(m_window, MouseScrollCallback); //Implement Later

	/* Checks if GLAD was loaded correctly */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/* OpenGL Debug CallBack */
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebugCallBack, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

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
	//glm::mat4 m_persProj = glm::perspective(glm::radians(fov), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 1000.0f);
	/* View Matrix */
	//glm::mat4 m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	/* Pass in the Shader Files */
	//Shader m_shader("Shaders/ShaderVertTest.vert", "Shaders/ShaderFragTest.frag");
	//Shader m_colorShader("Shaders/VertexShaderPrac.vert", "Shaders/FragmentShaderPrac.frag");

	/* Renderer */
	Renderer m_renderer;
	/* New Geometry Class */
	Geometry m_geometry;
	/* Shader Objects */
	Shader m_shader;
	Shader m_colorShader;
	Shader m_modelShader;
	Shader m_cowModelShader;

	m_shader.CompileShader("Shaders/ShaderVertTest.vert");
	m_shader.CompileShader("Shaders/ShaderFragTest.frag");

	m_colorShader.CompileShader("Shaders/BlobShader.vert");
	m_colorShader.CompileShader("Shaders/BlobShader.frag");

	m_modelShader.CompileShader("Shaders/ModelShaderVS.vert");
	m_modelShader.CompileShader("Shaders/ModelShaderFS.frag");
	
	m_cowModelShader.CompileShader("Shaders/CowModelShaderVS.vert");
	m_cowModelShader.CompileShader("Shaders/CowModelShaderFS.frag");

	/* Binds and UnBinds the Shader */
	m_shader.Link();
	m_shader.Validate();
	m_shader.Bind();
	m_shader.UnBind();
	m_shader.PrintActiveUniforms();
	m_shader.PrintActiveAttribs();

	/* Binds and Unbinds the Shader */
	m_colorShader.Link();
	m_colorShader.Validate();
	m_colorShader.Bind();
	m_colorShader.UniformBlock();
	m_colorShader.UnBind();
	m_colorShader.PrintActiveUniforms();
	m_colorShader.PrintActiveAttribs();

	Model m_cowModelLoad("Models/Cow Model/Statuette.obj");
	m_cowModelShader.Link();
	m_cowModelShader.Validate();
	m_cowModelShader.Bind();
	m_cowModelShader.UnBind();
	m_cowModelShader.PrintActiveUniforms();
	m_cowModelShader.PrintActiveAttribs();

	Model m_modelLoad("Models/Backpack Model/backpack.obj");
	m_modelShader.Link();
	m_modelShader.Validate();
	m_modelShader.Bind();
	m_modelShader.UnBind();
	m_modelShader.PrintActiveUniforms();
	m_modelShader.PrintActiveAttribs();

	/* Texture Location */
	Texture m_texture("Textures/Future City.png");
	Texture m_textuerTwo("Textures/Trying.png");

	/* Binds the Texture */
	/* If no argument was put in, it is automatically 0 */
	/* Assigns Texture to Slot 0 */
	m_texture.Bind(3);
	/* Binds to Texture Slot 1 */
	m_textuerTwo.Bind(2);

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
	glm::vec3 m_translationA(300.0f, 0.0f, 300.0f);
	glm::vec3 m_translationB(500.0f, 0.0f, 200.0f);
	glm::vec3 m_translationC(100.0f, 0.0f, 100.0f);
	glm::vec3 m_translationD(100.0f, 0.0f, 100.0f);
	glm::vec3 m_translationE(500.0f, 50.0f, 100.0f);

	/* (Orthographic Projection) Maps all our Coords on a 2D plane (Left, Right, Bottom, Top, Far, Near) */
	glm::mat4 m_proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -90.0f, 500.0f);

	/* Passes through the Screen Parameters (Since I dont know a better way this will do for now) */
	m_camera.SetScreenDimensions(SCR_HEIGHT, SCR_WIDTH);

	stbi_set_flip_vertically_on_load(true);

	bool m_buttonPressed = false;

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

		/* Calculates Delta and Last-Frame Time */
		float m_currentFrame = glfwGetTime();
		m_deltaTime = m_currentFrame - m_lastFrame;
		m_lastFrame = m_currentFrame;

		glm::mat4 m_persProj = glm::perspective(glm::radians(m_camera.GetCameraFOV()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

		m_camera.CameraInputs(m_window, m_cameraSpeedValue, m_deltaTime);

		/* Renders Two objects (Exact same object) */
		{
			/* Model Matrix */
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationA);
			m_model = glm::rotate(m_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mvp = m_camera.GetProjView() * m_camera.GetViewMatrix() * m_model;
			m_colorShader.Bind();
			//m_colorShader.SetUniform1i("u_Texture", 1);
			m_colorShader.SetUniformMat4f("u_MVP", m_mvp);
			m_geometry.GenerateQuad();
		}

		{
			/* Model Matrix */
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationB);
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mvp = m_camera.GetProjView() * m_camera.GetViewMatrix() * m_model;
			m_shader.Bind();
			m_shader.SetUniform1i("u_Texture", 3);
			m_shader.SetUniformMat4f("u_MVP", m_mvp);
			m_geometry.GenerateCube();
		}

		/* ImGui Window Render */
		{
			/* Edit 1 float using a slider from 0.0f to 960.0f */
			ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f);
			ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.0f, 960.0f);
			ImGui::SliderFloat3("Translation D", &m_translationD.x, 0.0f, 960.0f);
			ImGui::SliderFloat3("Translation E", &m_translationE.x, 0.0f, 960.0f);
			if (m_buttonPressed)
			{
				ImGui::SliderFloat3("Translation C", &m_translationC.x, 0.0f, 960.0f);
			}
			ImGui::SliderFloat("Camera Speed", &m_cameraSpeedValue, 0.0f, 500.0f);
			ImGui::SliderFloat("Camera Sensitivity", &m_sensitivity, 0.01f, 1.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		if (ImGui::Button("Spawn Cube", ImVec2(100.0f, 30.0f)))
		{
			m_buttonPressed = true;
		};

		if (m_buttonPressed)
		{
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationC);
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mvp = m_camera.GetProjView() * m_camera.GetViewMatrix() * m_model;
			m_shader.Bind();
			m_shader.SetUniform1i("u_Texture", 2);
			m_shader.SetUniformMat4f("u_MVP", m_mvp);
			m_geometry.GenerateCube();
		}

		{
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationD);
			m_model = glm::scale(m_model, glm::vec3(20.0f, 20.0f, 20.0f));
			glm::mat4 m_mvp = m_camera.GetProjView() * m_camera.GetViewMatrix() * m_model;
			m_modelShader.Bind();
			m_modelShader.SetUniformMat4f("u_MVP", m_mvp);
			m_modelLoad.DrawModel(m_modelShader);
		}

		{
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationE);
			m_model = glm::scale(m_model, glm::vec3(10.0f, 10.0f, 10.0f));
			glm::mat4 m_mvp = m_camera.GetProjView() * m_camera.GetViewMatrix() * m_model;
			m_cowModelShader.Bind();
			m_cowModelShader.SetUniformMat4f("u_MVP", m_mvp);
			m_cowModelLoad.DrawModel(m_modelShader);
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

void MouseCallback(GLFWwindow* a_window, double a_xpos, double a_ypos)
{
	/* Will find better solution but its to stop the weird jerk at the start */
	if (firstMouse)
	{
		m_lastX = a_xpos;
		m_lastY = a_ypos;

		firstMouse = false;
	}

	/* Calculate the OffSet Movement between the last and current frame */
	float m_xOffSet = a_xpos - m_lastX;
	float m_yOffSet = m_lastY - a_ypos; // Reversed since Y-Coordinates range from bottom to top
	m_lastX = a_xpos;
	m_lastY = a_ypos;

	/* Calculates Mouse Sensitivity */
	m_xOffSet *= m_sensitivity;
	m_yOffSet *= m_sensitivity;

	if (glfwGetMouseButton(m_window, 1) == GLFW_PRESS)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		m_camera.CameraRotation(m_xOffSet, m_yOffSet);
	}
	else
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void MouseScrollCallback(GLFWwindow* a_window, double a_XoffSet, double a_yOffSet)
{
	m_camera.CameraScroll(a_yOffSet);
}

std::string ErrorSourceToString(GLenum a_source)
{
	/* Switch Statement that contins the source information of the error */
	std::string m_errorStr;

	switch (a_source)
	{
	case GL_DEBUG_SOURCE_API:
		std::cout << "OPENGL_API_ERROR: ";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		m_errorStr = "WINDOWS_API_ERROR: ";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		m_errorStr = "THIRD_PARTY_ERROR: ";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		m_errorStr = "APPLICATION_ERROR: ";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		m_errorStr = "OTHER_SOURCE_ERROR: ";
		break;
	}
	std::cout << std::endl;
	return m_errorStr;
}

std::string ErrorTypeToString(GLenum a_source)
{
	/* Switch Statement that contins the type information of the error */
	std::string m_errorStr;

	switch (a_source)
	{
	case GL_DEBUG_TYPE_ERROR:
		m_errorStr = "OPENGL_API_ERROR: ";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		m_errorStr = "DEPRECIATED_ERROR: ";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		m_errorStr = "UNDERFINED_ERROR: ";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		m_errorStr = "PORTABILITY_ERROR: ";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		m_errorStr = "PERFORMANCE_ERROR: ";
		break;
	case GL_DEBUG_TYPE_MARKER:
		m_errorStr = "ANNOTAION: ";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		m_errorStr = "DEBUG_GROUP_PUSH: ";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		m_errorStr = "DEBUG_GROUP_POP: ";
		break;
	case GL_DEBUG_TYPE_OTHER:
		m_errorStr = "OTHER_MESSAGES: ";
		break;
	}
	std::cout << std::endl;
	return m_errorStr;
}

std::string ErrorSeverityToString(GLenum a_source)
{
	/* Switch Statement that contins the severity information of the error */
	std::string m_errorStr;

	switch (a_source)
	{
	case GL_DEBUG_SEVERITY_HIGH:  std::cout << "HIGH SEVERITY";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "MEDIUM_SEVERITY";
		break;
	case GL_DEBUG_SEVERITY_LOW: std::cout << "LOW_SEVERITY";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "DEBUG_NOTIFICATION";
		break;
	}
	std::cout << std::endl;

	return m_errorStr;
}

void DebugCallBack(GLenum a_source, GLenum a_type, GLuint a_ID,
	GLenum a_severity, GLsizei a_length, const GLchar* a_message,
	const void* a_param)
{
	/* Debug Callback Funtion for OpenGL (ignoring some ID codes due to spam) */
	if (a_ID == 131169 || a_ID == 131185 || a_ID == 131218 || a_ID == 131204) return;
	
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Debug_Message (" << a_ID << "): " << a_message << std::endl;

	//Convert GLenum params into strings
	//std::string m_sourceStr = ErrorSeverityToString(a_source);
	//std::string m_typeStr = ErrorTypeToString(a_type);
	//std::string m_severityStr = ErrorSeverityToString(a_severity);
	/* Fix Later */


	switch (a_severity)
	{
	case GL_DEBUG_SEVERITY_HIGH: std::cout << "SEVERITY: HIGH SEVERITY";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "SEVERITY: MEDIUM_SEVERITY";
		break;
	case GL_DEBUG_SEVERITY_LOW: std::cout << "SEVERITY: LOW_SEVERITY";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "SEVERITY: DEBUG_NOTIFICATION";
		break;
	}
	std::cout << std::endl;
	switch (a_type)
	{
	case GL_DEBUG_TYPE_ERROR: std::cout << "TYPE: ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "TYPE: DEPRECIATED_ERROR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "TYPE: UNDERFINED_ERROR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY: std::cout << "TYPE: PORTABILITY_ERROR";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "TYPE: PERFORMANCE_ERROR";
		break;
	case GL_DEBUG_TYPE_MARKER: std::cout << "TYPE: ANNOTAION";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "TYPE: DEBUG_GROUP_PUSH";
		break;
	case GL_DEBUG_TYPE_POP_GROUP: std::cout << "TYPE: DEBUG_GROUP_POP";
		break;
	case GL_DEBUG_TYPE_OTHER: std::cout << "TYPE: OTHER_MESSAGES";
		break;
	}
	std::cout << std::endl;
	switch (a_source)
	{
	case GL_DEBUG_SOURCE_API: std::cout << "SOURCE: OPENGL_API_ERROR";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "SOURCE: WINDOWS_API_ERROR";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "SOURCE: THIRD_PARTY_ERROR";
		break;
	case GL_DEBUG_SOURCE_APPLICATION: std::cout << "SOURCE: APPLICATION_ERROR";
		break;
	case GL_DEBUG_SOURCE_OTHER: std::cout << "SOURCE: OTHER_SOURCE_ERROR";
		break;
	}
	std::cout << std::endl;

	//printf(("%s:%s[%s] (%d): %s"), a_source, a_type, a_severity, a_ID, a_message);
}