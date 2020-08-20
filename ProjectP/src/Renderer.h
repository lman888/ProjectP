#pragma once

#include "GLAD/glad.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* a_function, const char* a_file, int a_line);

class Renderer
{
public:

	void Clear() const;
	void Draw(const VertexArray& a_va, const IndexBuffer& a_ib /*const Shader& a_shader*/) const;
};