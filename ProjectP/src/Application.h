#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Extra/Remotery/lib/Remotery.h"

#include "GLAD/glad.h"
#include <gl/GL.h>
#include "GLFW/glfw3.h"

#include "Model.h"
#include "Camera.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Application
{

public:
	
	/* Initialises the Window */
	int StartUp(void);

	/* Updates the Window and Objects in it */
	int Update();

	/* Stops the OpenGL Application */
	void Terminate();

	void ImGuiEvents();

	void ShaderCompilations();

protected:


private:

	Remotery* rmt;

	/* Objects Initial Position */
	#pragma region Objects Initial Position

	glm::vec3 m_translationA = glm::vec3(300.0f, 0.0f, 300.0f);
	glm::vec3 m_translationB = glm::vec3(500.0f, 0.0f, 200.0f);
	glm::vec3 m_translationC = glm::vec3(100.0f, 0.0f, 100.0f);
	glm::vec3 m_translationE = glm::vec3(500.0f, 50.0f, 100.0f);
	glm::vec3 m_translationF = glm::vec3(100.0f, 50.0f, 600.0f);
	glm::vec3 m_translationG = glm::vec3(150.0f, 50.0f, 300.0f);
	glm::vec3 m_translationH = glm::vec3(200.0f, 50.0f, 300.0f);
	glm::vec3 m_translationI = glm::vec3(300.0f, 10.0f, 400.0f);
	glm::vec3 m_translationJ = glm::vec3(500.0f, 10.0f, 700.0f);
	glm::vec3 m_translationK = glm::vec3(700.0f, 10.0f, 700.0f);
	glm::vec3 m_translationL = glm::vec3(300.0f, 200.0f, 100.0f);
	glm::vec3 m_translationM = glm::vec3(0.0f, 0.0f, -2.0f);
	glm::vec3 m_lightIntensity = glm::vec3(0.3f, 0.5f, 0.7f);
	glm::vec3 m_diffRefl = glm::vec3(0.3f, 0.5f, 0.7f);
	glm::vec3 m_teaPotrotation = glm::vec3(10.0f);
	//glm::vec4 m_lightPos(5.0f, 5.0f, 5.0f, 1.0f);
	glm::vec3 m_lidTransform = glm::vec3(0.0f, 1.5f, 0.25f);
	glm::vec3 m_lValue = glm::vec3(0.0f, 2.0f, 3.0f);
	glm::vec3 m_laValue = glm::vec3(0.0f, 2.0f, 3.0f);

	glm::vec4 m_worldLight = glm::vec4(5.0f, 5.0f, 2.0f, 1.0f);

	int m_sides = 20;
	int m_rings = 20;

	float m_outerRadius = 30;
	float m_innerRadius = 30;
	float m_radius = 10;

	int m_slices = 10;
	int m_stacks = 10;
	int m_teaPotGrid = 13;

	bool m_buttonPressed = false;
	bool m_lightPropWindow = false;

	glm::vec4 m_spotLight;

	float m_angle;

	#pragma endregion

	/* Shader Objects */
	#pragma region Shader variables

	/* Shader Objects */
	Shader m_shader;
	Shader m_colorShader;
	Shader m_cowModelShader;
	Shader m_PhongShader;
	Shader m_PhongReflectionShader;
	Shader m_PhongSubroutine;
	Shader m_realismPhongShader;
	Shader m_PhondTwoSidedShadingSub;
	Shader m_multipleLightsShader;
	Shader m_stormTrooperModelShader;
	Shader m_blingPhongShader;
	Shader m_spotLightShader;

	#pragma endregion

	#pragma region Model Loading


	#pragma endregion


};