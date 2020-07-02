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
