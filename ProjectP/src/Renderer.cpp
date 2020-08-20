#include "Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* a_function, const char* a_file, int a_line)
{
	if (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << a_function <<
			" " << a_file << ":" << a_line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	/* Renders the background colour */
	GLCall(glClearColor(0.2, 0.3f, 0.3f, 1.0f));

	/* When we call glClear, the entire colour buffer will be filled with the colour as configured by glClearColor */
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& a_va, const IndexBuffer& a_ib /*const Shader& a_shader*/) const
{
	/* Binds the Shader Program to the GPU */
	//a_shader.Bind();
	/* Binds the Vertex Array Buffer (This contains all of the Vertex Buffers data) */
	a_va.Bind();
	/* Binds our Index Buffer (contains the indices that are inside the Vertex Buffer,
	(chooses which indices we want to render on our screen)) */
	a_ib.Bind();

	/* Draws the Shape we defined with our Buffers */
	GLCall(glDrawElements(GL_TRIANGLES, a_ib.GetCount(), GL_UNSIGNED_INT, 0));
}