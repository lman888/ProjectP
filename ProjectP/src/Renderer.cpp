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

void Renderer::Draw(const VertexArray& a_va, const IndexBuffer& a_ib, const Shader& a_shader) const
{
	a_shader.Bind();
	/* Binds the Vertex Array Buffer (Which also binds the Vertex Buffer Object */
	a_va.Bind();
	/* Binds our Index Buffer */
	a_ib.Bind();

	/* Draws the Shape we defined with our Buffers */
	GLCall(glDrawElements(GL_TRIANGLES, a_ib.GetCount(), GL_UNSIGNED_INT, 0));
	/* Draws Triagnles in WireFrame Mode */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/* Sets the Triangles to its default look */
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}