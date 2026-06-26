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
	m_Vertex_buffer->bind();
	m_Index_buffer->bind();
}

//void VertexArray::unbind() const {
//	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
//}

void VertexArray::addBuffer(const VertexBuffer*  VBO){
	m_Vertex_buffer = VBO;
	//VBO.bind();
};


void VertexArray::addIndexes(const IndexBuffer* EBO){
	m_Index_buffer = EBO;
};
