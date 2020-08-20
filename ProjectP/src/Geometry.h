#pragma once


#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Geometry
{
public:

	Geometry();
	~Geometry();

	void GenerateQuad();

	void GenerateCube();

protected:

private:

};

