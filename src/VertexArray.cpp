#include "Renderer.h"
#include "VertexArray.h"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_Renderer_id));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_Renderer_id));
}

void VertexArray::bind() const {
	GLCall(glBindVertexArray(m_Renderer_id));

	if (m_Vertex_buffer != nullptr) {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_buffer->get_id()));
	}

	if (m_Index_buffer != nullptr) {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_buffer->get_id()));
	}
}

//void VertexArray::unbind() const {
//	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
//}

void VertexArray::addBuffer(const VertexBuffer* VBO){
	m_Vertex_buffer = VBO;
	m_Vertex_buffer->bind();
};


void VertexArray::addIndexes(const IndexBuffer* EBO){
	m_Index_buffer = EBO;
	m_Index_buffer->bind();
};
