#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& a_VB, const VertexBufferLayout& a_Layout)
{
	/* Binds the Vertex Array */
	Bind();
	/* Binds the Buffer we want to work with */
	a_VB.Bind();

	/* Sets up the layout for the Vertex Array */
	const auto& Elements = a_Layout.GetElements();
	unsigned int OffSet = 0;

	for(unsigned int i = 0; i < Elements.size(); i++)
	{
		/* Iterates through the Elements */
		const auto& element = Elements[i];
		//* Sets the Vertex Attribute Pointers */
		GLCall(glEnableVertexAttribArray(i));
		/* Specifies the layout of the Vertex Buffer with the data provided*/
		GLCall(glVertexAttribPointer(i, element.m_Count, element.m_Type,
		element.m_Noramlized, a_Layout.GetStride(), (const void*)OffSet));

		/* Checks the Size of each Type */
		OffSet += element.m_Count * VertexBufferLayoutElement::GetSizeOfType(element.m_Type);
	}
}

void VertexArray::Bind() const
{
	//* Binds the VAO (Vertex Array Object) From this point onwards, we can bind/configure the - */
	//* Corresponding VBO(s) and attribute pointer(s) and then unbind the VAO for later use */
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}
