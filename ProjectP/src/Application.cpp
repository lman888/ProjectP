#include "Application.h"
#include <signal.h>

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

void AggregrateFunc()
{
	rmt_BeginCPUSample(aggregate, RMTSF_Aggregate);
	rmt_EndCPUSample();
}

void RecursiveFunction(int a_depth)
{
	rmt_BeginCPUSample(recursive, RMTSF_Recursive);

	if (a_depth < 5)
	{
		RecursiveFunction(a_depth + 1);
	}

	rmt_EndCPUSample();
}

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

	// You need only do this once per program.
	rmtSettings m_settings = rmtSettings();
	rmtError m_rmtError;

	/* Renderer */
	Renderer m_renderer;
	/* New Geometry Class */
	Geometry m_geometry;

	/* Compiles the huge Shader Chunk */
	ShaderCompilations();

	Model m_cowModelLoad("Models/Cow Model/Statuette.obj");
	m_cowModelShader.Link();
	m_cowModelShader.Validate();
	m_cowModelShader.Bind();
	m_cowModelShader.UnBind();

	Model m_stormTrooperModel("Models/Backpack/backpack.obj");
	m_stormTrooperModelShader.Link();
	m_stormTrooperModelShader.Validate();
	m_stormTrooperModelShader.Bind();
	m_stormTrooperModelShader.UnBind();


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

	/* (Orthographic Projection) Maps all our Coords on a 2D plane (Left, Right, Bottom, Top, Far, Near) */
	glm::mat4 m_proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -90.0f, 500.0f);

	/* Passes through the Screen Parameters (Since I dont know a better way this will do for now) */
	m_camera.SetScreenDimensions(SCR_HEIGHT, SCR_WIDTH);

	stbi_set_flip_vertically_on_load(true);

	float my_color[4];

	m_settings.malloc;
	m_settings.free;
	m_settings.mm_context;

	m_settings.input_handler;
	m_settings.input_handler_context;

	// Create the main instance of Remotery.
	rmt_BindOpenGL();

	m_rmtError = rmt_CreateGlobalInstance(&rmt);

	if (RMT_ERROR_NONE != m_rmtError) {
		printf("Error launching Remotery %d\n", m_rmtError);
		return -1;
	}

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

		glm::mat4 m_persProj = glm::perspective(glm::radians(m_camera.GetCameraFOV()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 3500.0f);

		m_camera.SetProjView(m_persProj);

		m_camera.CameraInputs(m_window, m_cameraSpeedValue, m_deltaTime);

		rmt_BeginOpenGLSample();
		rmt_BeginCPUSample(MainRenderLoop, 0);
		rmt_LogText("Main Loop");

		/* Renders Two objects (Exact same object) */
		#pragma region ColorShader

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(QuadRender, 0);
			rmt_LogText("Quad Render");
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
			rmt_EndCPUSample();
		}

		#pragma endregion
		
		#pragma region TexturedShader

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(CubeRender, 0);
			rmt_LogText("Cube Render");
			/* Model Matrix */
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationB);
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mvp = m_camera.GetProjView() * m_camera.GetViewMatrix() * m_model;
			m_shader.Bind();
			m_shader.SetUniform1i("u_Texture", 3);
			m_shader.SetUniformMat4f("u_MVP", m_mvp);
			m_geometry.GenerateCube();
			rmt_EndCPUSample();
		}


		#pragma endregion
		
		#pragma region CowModel

		{

			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(CowModelRender, 0);
			rmt_LogText("Cow Model Render");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationE);
			m_model = glm::scale(m_model, glm::vec3(10.0f, 10.0f, 10.0f));
			glm::mat4 m_mvp = m_camera.GetProjView() * m_camera.GetViewMatrix() * m_model;
			m_cowModelShader.Bind();
			m_cowModelShader.SetUniformMat4f("u_MVP", m_mvp);
			m_cowModelLoad.DrawModel(m_cowModelShader);
			rmt_EndCPUSample();
		}

		#pragma endregion

		#pragma region AmongUsModel

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(StormTrooper, 0);
			rmt_LogText("Among Us Model Render");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationK);
			m_model = glm::scale(m_model, glm::vec3(10.0f, 10.0f, 10.0f));
			glm::mat4 m_mvp = m_camera.GetProjView() * m_camera.GetViewMatrix() * m_model;
			m_stormTrooperModelShader.Bind();
			m_stormTrooperModelShader.SetUniformMat4f("u_MVP", m_mvp);
			m_stormTrooperModel.DrawModel(m_stormTrooperModelShader);
			rmt_EndCPUSample();
		}

		#pragma endregion

		/* ImGui Window Render */
		ImGuiEvents();

		#pragma region PhongShader

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(CircleOneRender, 0);
			rmt_LogText("Circle 1");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationC);
			//m_model = glm::rotate(m_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mv = m_camera.GetViewMatrix() * m_model;
			glm::mat4 m_projView = m_camera.GetProjView();
			glm::mat3 m_normMatrix = glm::mat3(glm::vec3(m_mv[0]), glm::vec3(m_mv[1]), glm::vec3(m_mv[2]));
			m_PhongShader.Bind();
			m_PhongShader.SetUniformVec3f("u_kd", m_diffRefl);
			m_PhongShader.SetUniformVec3f("u_ld", m_lightIntensity);
			m_PhongShader.SetUniformVec4f("u_lightPosition", m_camera.GetViewMatrix() * m_worldLight);
			m_PhongShader.SetUniformMat4f("u_modelViewMatrix", m_mv);
			m_PhongShader.SetUniformMat3f("u_normalMatrix", m_normMatrix);
			m_PhongShader.SetUniformMat4f("u_MVP", m_projView * m_mv);
			m_geometry.GenerateSphere(m_radius, (unsigned int)m_slices, (unsigned int)m_stacks);
			rmt_EndCPUSample();
		}

		#pragma endregion

		#pragma region Sun Object

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(CircleTwoRender, 0);
			rmt_LogText("Circle 2");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), (glm::vec3)m_worldLight);
			//m_model = glm::rotate(m_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mv = m_camera.GetViewMatrix() * m_model;
			glm::mat4 m_projView = m_camera.GetProjView();
			glm::mat3 m_normMatrix = glm::mat3(glm::vec3(m_mv[0]), glm::vec3(m_mv[1]), glm::vec3(m_mv[2]));
			m_PhongReflectionShader.Bind();
			m_PhongReflectionShader.SetUniformVec3f("Material.u_kd", m_diffRefl);
			m_PhongReflectionShader.SetUniformVec3f("Light.u_ld", m_lightIntensity);
			m_PhongReflectionShader.SetUniformVec4f("Light.u_position", m_camera.GetViewMatrix() * m_worldLight);
			m_PhongReflectionShader.SetUniformMat4f("u_modelViewMatrix", m_mv);
			m_PhongReflectionShader.SetUniformMat3f("u_normalMatrix", m_normMatrix);
			m_PhongReflectionShader.SetUniformMat4f("u_MVP", m_projView * m_mv);
			m_geometry.GenerateSphere(m_radius, (unsigned int)m_slices, (unsigned int)m_stacks);
			rmt_EndCPUSample();
		}

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(CircleTwoRender, 0);
			rmt_LogText("Circle 2");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), (glm::vec3)m_spotLight);
			//m_model = glm::rotate(m_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mv = m_camera.GetViewMatrix() * m_model;
			glm::mat4 m_projView = m_camera.GetProjView();
			glm::mat3 m_normMatrix = glm::mat3(glm::vec3(m_mv[0]), glm::vec3(m_mv[1]), glm::vec3(m_mv[2]));
			m_spotLightShader.Bind();
			m_spotLightShader.SetUniformVec3f("Material.u_kd", m_diffRefl);
			m_spotLightShader.SetUniformVec4f("Light.u_position", m_camera.GetViewMatrix() * m_spotLight);
			m_spotLightShader.SetUniformMat4f("u_modelViewMatrix", m_mv);
			m_spotLightShader.SetUniformMat3f("u_normalMatrix", m_normMatrix);
			m_spotLightShader.SetUniformMat4f("u_MVP", m_projView * m_mv);
			m_geometry.GenerateSphere(m_radius, (unsigned int)m_slices, (unsigned int)m_stacks);
			rmt_EndCPUSample();
		}

		#pragma endregion

		#pragma region PhongReflectionShader

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(CircleTwoRender, 0);
			rmt_LogText("Circle 2");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationF);
			//m_model = glm::rotate(m_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mv = m_camera.GetViewMatrix() * m_model;
			glm::mat4 m_projView = m_camera.GetProjView();
			glm::mat3 m_normMatrix = glm::mat3(glm::vec3(m_mv[0]), glm::vec3(m_mv[1]), glm::vec3(m_mv[2]));
			m_PhongReflectionShader.Bind();
			m_PhongReflectionShader.SetUniformVec3f("Material.u_kd", m_diffRefl);
			m_PhongReflectionShader.SetUniformVec3f("Light.u_ld", m_lightIntensity);
			m_PhongReflectionShader.SetUniformVec4f("Light.u_position", m_camera.GetViewMatrix() * m_worldLight);
			m_PhongReflectionShader.SetUniformMat4f("u_modelViewMatrix", m_mv);
			m_PhongReflectionShader.SetUniformMat3f("u_normalMatrix", m_normMatrix);
			m_PhongReflectionShader.SetUniformMat4f("u_MVP", m_projView * m_mv);
			m_geometry.GenerateSphere(m_radius, (unsigned int)m_slices, (unsigned int)m_stacks);
			rmt_EndCPUSample();
		}

		#pragma endregion

		#pragma region Subroutine Shader

		GLuint m_phongIndex = glGetSubroutineIndex(m_PhongSubroutine.GetHandle(), GL_VERTEX_SHADER, "PhongModel");
		GLuint m_diffuseIndex = glGetSubroutineIndex(m_PhongSubroutine.GetHandle(), GL_VERTEX_SHADER, "DiffuseOnly");

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(TeaPotRender, 0);
			rmt_LogText("Tea Pot Render");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationG);
			//m_model = glm::rotate(m_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mv = m_camera.GetViewMatrix() * m_model;
			glm::mat4 m_projView = m_camera.GetProjView();
			glm::mat3 m_normMatrix = glm::mat3(glm::vec3(m_mv[0]), glm::vec3(m_mv[1]), glm::vec3(m_mv[2]));
			m_PhongSubroutine.Bind();
			glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &m_phongIndex);
			m_PhongSubroutine.SetUniformVec3f("Material.u_kd", m_diffRefl);
			m_PhongSubroutine.SetUniformVec3f("Light.u_ld", m_lightIntensity);
			m_PhongSubroutine.SetUniformVec4f("Light.u_position", m_camera.GetViewMatrix() * m_worldLight);
			m_PhongSubroutine.SetUniformMat4f("u_modelViewMatrix", m_mv);
			m_PhongSubroutine.SetUniformMat3f("u_normalMatrix", m_normMatrix);
			m_PhongSubroutine.SetUniformMat4f("u_MVP", m_projView * m_mv);
			m_geometry.GenerateTeaPot(m_teaPotGrid, glm::translate(glm::mat4(1.0f), m_lidTransform));
			rmt_EndCPUSample();
		}

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(TeaPotRender2, 0);
			rmt_LogText("Tea Pot 2 Render");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationH);
			//m_model = glm::rotate(m_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mv = m_camera.GetViewMatrix() * m_model;
			glm::mat4 m_projView = m_camera.GetProjView();
			glm::mat3 m_normMatrix = glm::mat3(glm::vec3(m_mv[0]), glm::vec3(m_mv[1]), glm::vec3(m_mv[2]));
			m_PhongSubroutine.Bind();
			glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &m_diffuseIndex);
			m_PhongSubroutine.SetUniformVec3f("Material.u_kd", m_diffRefl);
			m_PhongSubroutine.SetUniformVec3f("Light.u_ld", m_lightIntensity);
			m_PhongSubroutine.SetUniformVec4f("Light.u_position", m_camera.GetViewMatrix() * m_worldLight);
			m_PhongSubroutine.SetUniformMat4f("u_modelViewMatrix", m_mv);
			m_PhongSubroutine.SetUniformMat3f("u_normalMatrix", m_normMatrix);
			m_PhongSubroutine.SetUniformMat4f("u_MVP", m_projView * m_mv);
			m_geometry.GenerateTeaPot(m_teaPotGrid, glm::translate(glm::mat4(1.0f), m_lidTransform));
			//m_geometry.GenerateTorus(m_outerRadius, m_innerRadius, m_sides, m_rings);
			rmt_EndCPUSample();
		}

		#pragma endregion

		#pragma region BlingPhongShader

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(TeaPotRender2, 0);
			rmt_LogText("Tea Pot 2 Render");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationL);
			//m_model = glm::rotate(m_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mv = m_camera.GetViewMatrix() * m_model;
			glm::mat4 m_projView = m_camera.GetProjView();
			glm::mat3 m_normMatrix = glm::mat3(glm::vec3(m_mv[0]), glm::vec3(m_mv[1]), glm::vec3(m_mv[2]));
			m_blingPhongShader.Bind();
			m_blingPhongShader.SetUniformVec3f("Material.u_kd", m_diffRefl);
			m_blingPhongShader.SetUniformVec3f("Light.u_l", m_lightIntensity);
			m_blingPhongShader.SetUniformVec4f("Light.u_position", m_camera.GetViewMatrix() * m_worldLight);
			m_blingPhongShader.SetUniformMat4f("u_modelViewMatrix", m_mv);
			m_blingPhongShader.SetUniformMat3f("u_normalMatrix", m_normMatrix);
			m_blingPhongShader.SetUniformMat4f("u_MVP", m_projView * m_mv);
			m_geometry.GenerateTeaPot(m_teaPotGrid, glm::translate(glm::mat4(1.0f), m_lidTransform));
			rmt_EndCPUSample();
		}

		#pragma endregion

		#pragma region Multiple Lights Shader

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(TorusRender, 0);
			rmt_LogText("Torus Render");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationJ);
			//m_model = glm::rotate(m_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mv = m_camera.GetViewMatrix() * m_model;
			glm::mat4 m_projView = m_camera.GetProjView();
			glm::mat3 m_normMatrix = glm::mat3(glm::vec3(m_mv[0]), glm::vec3(m_mv[1]), glm::vec3(m_mv[2]));
			m_multipleLightsShader.Bind();
			m_multipleLightsShader.SetUniformVec3f("Material.u_kd", m_diffRefl);
			m_multipleLightsShader.SetUniformMat4f("u_modelViewMatrix", m_mv);
			m_multipleLightsShader.SetUniformMat3f("u_normalMatrix", m_normMatrix);
			m_multipleLightsShader.SetUniformMat4f("u_MVP", m_projView * m_mv);

			m_multipleLightsShader.SetUniformVec3f("Lights[0].u_l", m_lValue);
			m_multipleLightsShader.SetUniformVec3f("Lights[1].u_l", m_lValue);
			m_multipleLightsShader.SetUniformVec3f("Lights[2].u_l", m_lValue);
			m_multipleLightsShader.SetUniformVec3f("Lights[3].u_l", m_lValue);
			m_multipleLightsShader.SetUniformVec3f("Lights[4].u_l", m_lValue);


			m_multipleLightsShader.SetUniformVec3f("Lights[0].u_la", m_laValue);
			m_multipleLightsShader.SetUniformVec3f("Lights[1].u_la", m_laValue);
			m_multipleLightsShader.SetUniformVec3f("Lights[2].u_la", m_laValue);
			m_multipleLightsShader.SetUniformVec3f("Lights[3].u_la", m_laValue);
			m_multipleLightsShader.SetUniformVec3f("Lights[4].u_la", m_laValue);

			m_geometry.GenerateTorus(m_outerRadius, m_innerRadius, m_sides, m_rings);
			rmt_EndCPUSample();
		}

		#pragma endregion

		#pragma region Realism Phong Shader

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(TeaPotRender2, 0);
			rmt_LogText("Tea Pot 2 Render");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationI);
			//m_model = glm::rotate(m_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mv = m_camera.GetViewMatrix() * m_model;
			glm::mat4 m_projView = m_camera.GetProjView();
			glm::mat3 m_normMatrix = glm::mat3(glm::vec3(m_mv[0]), glm::vec3(m_mv[1]), glm::vec3(m_mv[2]));
			m_realismPhongShader.Bind();
			m_realismPhongShader.SetUniformVec3f("Material.u_kd", m_diffRefl);
			m_realismPhongShader.SetUniformVec3f("Light.u_ld", m_lightIntensity);
			m_realismPhongShader.SetUniformVec4f("Light.u_position", m_camera.GetViewMatrix() * m_worldLight);
			m_realismPhongShader.SetUniformMat4f("u_modelViewMatrix", m_mv);
			m_realismPhongShader.SetUniformMat3f("u_normalMatrix", m_normMatrix);
			m_realismPhongShader.SetUniformMat4f("u_MVP", m_projView * m_mv);
			m_geometry.GenerateTeaPot(m_teaPotGrid, glm::translate(glm::mat4(1.0f), m_lidTransform));
			rmt_EndCPUSample();
		}

		#pragma endregion

		#pragma region SpotLight Shader

		{
			rmt_BeginOpenGLSample();
			rmt_BeginCPUSample(TeaPotRender2, 0);
			rmt_LogText("Tea Pot 2 Render");
			glm::mat4 m_model = glm::translate(glm::mat4(1.0f), m_translationM);
			//m_model = glm::rotate(m_model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			/* Model View Projection Calculation */
			/* (In OpenGL its Projection View Model) */
			glm::mat4 m_mv = m_camera.GetViewMatrix() * m_model;
			glm::mat4 m_projView = m_camera.GetProjView();
			glm::mat3 m_normMatrix = glm::mat3(glm::vec3(m_mv[0]), glm::vec3(m_mv[1]), glm::vec3(m_mv[2]));
			m_spotLightShader.Bind();
			m_spotLightShader.SetUniformVec3f("Material.u_kd", m_diffRefl);
			m_spotLightShader.SetUniformVec3f("Spot.u_position", m_camera.GetViewMatrix() * m_spotLight);
			m_spotLightShader.SetUniformVec3f("Spot.u_direction", m_normMatrix * glm::vec3(-m_spotLight));
			m_spotLightShader.SetUniformMat4f("u_modelViewMatrix", m_mv);
			m_spotLightShader.SetUniformMat3f("u_normalMatrix", m_normMatrix);
			m_spotLightShader.SetUniformMat4f("u_MVP", m_projView * m_mv);
			m_geometry.GenerateTeaPot(m_teaPotGrid, glm::translate(glm::mat4(1.0f), m_lidTransform));
			rmt_EndCPUSample();
		}

		#pragma endregion

		ImGui::End();

		rmt_EndCPUSample();

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
	rmt_DestroyGlobalInstance(rmt);
	rmt_UnbindOpenGL();
	glfwTerminate();
}

void Application::ImGuiEvents()
{
	#pragma region ImGui Translation Values
	{

		/* Edit 1 float using a slider from 0.0f to 960.0f */
		ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation E", &m_translationE.x, 0.0f, 960.0f);

		ImGui::SliderFloat("Camera Speed", &m_cameraSpeedValue, 0.0f, 500.0f);
		ImGui::SliderFloat("Camera Sensitivity", &m_sensitivity, 0.01f, 1.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}
	#pragma endregion

	if (ImGui::Button("Spawn Light Objects", ImVec2(150.0f, 30.0f)))
	{
		m_buttonPressed = true;
	};

	if (m_buttonPressed)
	{
		m_angle += 0.25f * m_deltaTime;

		if (m_angle > glm::two_pi<float>())
		{
			m_angle -= glm::two_pi<float>();
		}

		#pragma region ImGui Translation Values

		// Create a window called "My First Tool", with a menu bar.
		ImGui::Begin("Light Properties", &m_lightPropWindow, ImGuiWindowFlags_MenuBar);

		ImGui::SliderFloat3("Translation C", &m_translationC.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation F", &m_translationF.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation G", &m_translationG.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation H", &m_translationH.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation I", &m_translationI.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation J", &m_translationJ.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation K", &m_translationK.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation L", &m_translationL.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation M", &m_translationM.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Torus Inner Rings", &m_innerRadius, 0.0f, 500.0f);
		ImGui::SliderFloat("Torus Outer Rings", &m_outerRadius, 0.0f, 500.0f);
		ImGui::SliderInt("Torus Rings", &m_rings, 20, 200);
		/* Very quick and rough fix for the crashing if the Rings exceed the Sides */
		if (m_rings < m_sides)
		{
			m_rings = m_sides;
		}
		ImGui::SliderInt("Torus Sides", &m_sides, 20, m_rings);
		ImGui::SliderFloat3("Ambient Light", &m_laValue.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("Diffuse and Spec Intensity", &m_lValue.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("Lid Position", &m_lidTransform.x, 0.0f, 10.0f);
		ImGui::SliderFloat3("World Light Object", &m_worldLight.x, -1.0f, 360.0f);
		ImGui::SliderFloat3("Spot Light Object", &m_spotLight.x, -1.0f, 360.0f);
		ImGui::SliderFloat3("Light Intensity", &m_lightIntensity.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("Light Reflection", &m_diffRefl.x, 0.0f, 1.0f);
		ImGui::SliderFloat("Radius", &m_radius, 10.0f, 100.0f);
		//ImGui::SliderFloat("Cut Off", &m_cutOff, 1.0f, 1.50f);
		//ImGui::SliderFloat("Exponent", &m_exponent, 10.0f, 100.0f);
		ImGui::SliderInt("Slices", &m_slices, 6, 100);
		ImGui::SliderInt("Slaces", &m_stacks, 6, 100);
		ImGui::SliderInt("Teap Pot Grid", &m_teaPotGrid, 1, 100);

		#pragma endregion
	}
}

void Application::ShaderCompilations()
{
	#pragma region Shader Compilation

	m_shader.CompileShader("Shaders/ShaderVertTest.vert");
	m_shader.CompileShader("Shaders/ShaderFragTest.frag");

	m_colorShader.CompileShader("Shaders/BlobShader.vert");
	m_colorShader.CompileShader("Shaders/BlobShader.frag");

	m_cowModelShader.CompileShader("Shaders/CowModelShaderVS.vert");
	m_cowModelShader.CompileShader("Shaders/CowModelShaderFS.frag");

	m_stormTrooperModelShader.CompileShader("Shaders/CowModelShaderVS.vert");
	m_stormTrooperModelShader.CompileShader("Shaders/CowModelShaderFS.frag");

	m_PhongShader.CompileShader("Shaders/PhongLightingVert.vert");
	m_PhongShader.CompileShader("Shaders/PhongLightingFrag.frag");

	m_PhongReflectionShader.CompileShader("Shaders/PhongReflectionLighting.vert");
	m_PhongReflectionShader.CompileShader("Shaders/PhongReflectionLighting.frag");

	m_PhongSubroutine.CompileShader("Shaders/PhongTwoSidedShadingSub.vert");
	m_PhongSubroutine.CompileShader("Shaders/PhongTwoSidedShadingSub.frag");

	m_realismPhongShader.CompileShader("Shaders/PhongRealismShader.vert");
	m_realismPhongShader.CompileShader("Shaders/PhongRealismShader.frag");

	m_multipleLightsShader.CompileShader("Shaders/MultipleLightShader.vert");
	m_multipleLightsShader.CompileShader("Shaders/MultipleLightShader.frag");

	m_blingPhongShader.CompileShader("Shaders/BlinnPhong.vert");
	m_blingPhongShader.CompileShader("Shaders/BlinnPhong.frag");

	m_spotLightShader.CompileShader("Shaders/SpotLightShader.vert");
	m_spotLightShader.CompileShader("Shaders/SpotLightShader.frag");

	/* Checks for Linking, Validating that link, Binding and Unbinding the Shaders */

	m_blingPhongShader.Link();
	m_blingPhongShader.Validate();
	m_blingPhongShader.Bind();
	m_blingPhongShader.UnBind();

	m_PhongReflectionShader.Link();
	m_PhongReflectionShader.Validate();
	m_PhongReflectionShader.Bind();
	m_PhongReflectionShader.UnBind();

	m_PhongSubroutine.Link();
	m_PhongSubroutine.Validate();
	m_PhongSubroutine.Bind();
	m_PhongSubroutine.UnBind();

	m_PhongShader.Link();
	m_PhongShader.Validate();
	m_PhongShader.Bind();
	m_PhongShader.UnBind();

	m_shader.Link();
	m_shader.Validate();
	m_shader.Bind();
	m_shader.UnBind();

	m_colorShader.Link();
	m_colorShader.Validate();
	m_colorShader.Bind();
	m_colorShader.UniformBlock();
	m_colorShader.UnBind();

	m_realismPhongShader.Link();
	m_realismPhongShader.Validate();
	m_realismPhongShader.Bind();
	m_realismPhongShader.UnBind();

	m_multipleLightsShader.Link();
	m_multipleLightsShader.Validate();
	m_multipleLightsShader.Bind();
	m_multipleLightsShader.UnBind();

	m_spotLightShader.Link();
	m_spotLightShader.Validate();
	m_spotLightShader.Bind();
	m_spotLightShader.UnBind();

	#pragma endregion

	#pragma region Phong Lighting Settings

	m_angle = 0.0f;
	m_spotLight = glm::vec4(10.0f * cos(m_angle), 1.0f, 1.0f * sin(m_angle), 1.0f);

	m_PhongReflectionShader.Bind();
	m_PhongReflectionShader.SetUniformVec3f("Material.u_kd", glm::vec3(0.9f, 0.5f, 0.3f));
	m_PhongReflectionShader.SetUniformVec3f("Light.u_ld", glm::vec3(1.0f, 1.0f, 1.0f));
	m_PhongReflectionShader.SetUniformVec4f("Light.u_position", m_camera.GetViewMatrix() * m_worldLight);
	m_PhongReflectionShader.SetUniformVec3f("Material.u_ka", glm::vec3(0.9f, 0.5f, 0.3f));
	m_PhongReflectionShader.SetUniformVec3f("Light.u_la", glm::vec3(0.4f, 0.4f, 0.4f));
	m_PhongReflectionShader.SetUniformVec3f("Material.u_ks", glm::vec3(0.8f, 0.8f, 0.8f));
	m_PhongReflectionShader.SetUniformVec3f("Light.u_ls", glm::vec3(1.0f, 1.0f, 1.0f));
	m_PhongReflectionShader.SetUniform1f("Material.u_shininess", 100.0f);
	m_PhongReflectionShader.UnBind();

	m_realismPhongShader.Bind();
	m_realismPhongShader.SetUniformVec3f("Material.u_kd", glm::vec3(0.9f, 0.5f, 0.3f));
	m_realismPhongShader.SetUniformVec3f("Light.u_ld", glm::vec3(1.0f, 1.0f, 1.0f));
	m_realismPhongShader.SetUniformVec4f("Light.u_position", m_camera.GetViewMatrix() * m_worldLight);
	m_realismPhongShader.SetUniformVec3f("Material.u_ka", glm::vec3(0.9f, 0.5f, 0.3f));
	m_realismPhongShader.SetUniformVec3f("Light.u_la", glm::vec3(0.4f, 0.4f, 0.4f));
	m_realismPhongShader.SetUniformVec3f("Material.u_ks", glm::vec3(0.8f, 0.8f, 0.8f));
	m_realismPhongShader.SetUniformVec3f("Light.u_ls", glm::vec3(1.0f, 1.0f, 1.0f));
	m_realismPhongShader.SetUniform1f("Material.u_shininess", 100.0f);
	m_realismPhongShader.UnBind();

	m_PhongSubroutine.Bind();
	m_PhongSubroutine.SetUniformVec3f("Material.u_kd", glm::vec3(0.9f, 0.5f, 0.3f));
	m_PhongSubroutine.SetUniformVec3f("Light.u_ld", glm::vec3(1.0f, 1.0f, 1.0f));
	m_PhongSubroutine.SetUniformVec4f("Light.u_position", m_camera.GetViewMatrix() * m_worldLight);
	m_PhongSubroutine.SetUniformVec3f("Material.u_ka", glm::vec3(0.9f, 0.5f, 0.3f));
	m_PhongSubroutine.SetUniformVec3f("Light.u_la", glm::vec3(0.4f, 0.4f, 0.4f));
	m_PhongSubroutine.SetUniformVec3f("Material.u_ks", glm::vec3(0.8f, 0.8f, 0.8f));
	m_PhongSubroutine.SetUniformVec3f("Light.u_ls", glm::vec3(1.0f, 1.0f, 1.0f));
	m_PhongSubroutine.SetUniform1f("Material.u_shininess", 100.0f);
	m_PhongSubroutine.UnBind();

	m_blingPhongShader.Bind();
	m_blingPhongShader.SetUniformVec3f("Material.u_kd", glm::vec3(0.9f, 0.5f, 0.3f));
	m_blingPhongShader.SetUniformVec4f("Light.u_position", m_camera.GetViewMatrix() * m_worldLight);
	m_blingPhongShader.SetUniformVec3f("Material.u_ka", glm::vec3(0.9f, 0.5f, 0.3f));
	m_blingPhongShader.SetUniformVec3f("Light.u_la", glm::vec3(0.4f, 0.4f, 0.4f));
	m_blingPhongShader.SetUniformVec3f("Light.u_l", glm::vec3(0.0f, 0.8f, 0.8f));
	m_blingPhongShader.SetUniformVec3f("Material.u_ks", glm::vec3(0.8f, 0.8f, 0.8f));
	m_blingPhongShader.SetUniform1f("Material.u_shininess", 100.0f);
	m_blingPhongShader.UnBind();

	m_spotLightShader.Bind();
	m_spotLightShader.SetUniformVec3f("Material.u_kd", glm::vec3(0.9f, 0.5f, 0.3f));
	m_spotLightShader.SetUniformVec3f("Spot.u_position", m_camera.GetViewMatrix() * m_spotLight);
	m_spotLightShader.SetUniformVec3f("Material.u_ka", glm::vec3(0.9f, 0.5f, 0.3f));
	m_spotLightShader.SetUniformVec3f("Spot.u_la", glm::vec3(0.5f));
	m_spotLightShader.SetUniformVec3f("Spot.u_l", glm::vec3(0.9f));
	m_spotLightShader.SetUniform1f("Spot.u_exponent", 50.0f);
	m_spotLightShader.SetUniform1f("Spot.u_cutOff", glm::radians(15.0f));
	m_spotLightShader.SetUniformVec3f("Material.u_ks", glm::vec3(0.8f, 0.8f, 0.8f));
	m_spotLightShader.SetUniform1f("Material.u_shininess", 100.0f);
	m_spotLightShader.UnBind();

	#pragma endregion

	#pragma region Multiple Light Shader Settings

	m_multipleLightsShader.Bind();

	float x, y;
	for (int i = 0; i < 5; i++)
	{
		std::stringstream m_name;
		m_name << "Lights[" << i << "].u_position";

		x = 2.0f * cosf((glm::two_pi<float>() / 5) * i);
		y = 2.0f * sinf((glm::two_pi<float>() / 5) * i);

		m_multipleLightsShader.SetUniformVec4f(m_name.str().c_str(), m_camera.GetViewMatrix() * glm::vec4(x, 1.2f, y + 1.0f, 1.0f));
	}

	m_multipleLightsShader.SetUniformVec3f("Lights[0].u_l", glm::vec3(0.8f, 0.8f, 0.8f));
	m_multipleLightsShader.SetUniformVec3f("Lights[1].u_l", glm::vec3(0.5f, 0.5f, 0.5f));
	m_multipleLightsShader.SetUniformVec3f("Lights[2].u_l", glm::vec3(0.0f, 0.1f, 0.1f));
	m_multipleLightsShader.SetUniformVec3f("Lights[3].u_l", glm::vec3(0.3f, 0.3f, 0.3f));
	m_multipleLightsShader.SetUniformVec3f("Lights[4].u_l", glm::vec3(0.0f, 0.8f, 0.8f));


	m_multipleLightsShader.SetUniformVec3f("Lights[0].u_la", glm::vec3(0.0f, 0.2f, 0.2f));
	m_multipleLightsShader.SetUniformVec3f("Lights[1].u_la", glm::vec3(0.0f, 0.0f, 0.6f));
	m_multipleLightsShader.SetUniformVec3f("Lights[2].u_la", glm::vec3(0.7f, 0.7f, 0.7f));
	m_multipleLightsShader.SetUniformVec3f("Lights[3].u_la", glm::vec3(0.0f, 0.2f, 0.0f));
	m_multipleLightsShader.SetUniformVec3f("Lights[4].u_la", glm::vec3(0.2f, 0.2f, 0.2f));

	m_multipleLightsShader.UnBind();

	#pragma endregion
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