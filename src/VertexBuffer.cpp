#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const float* vertices, const int size) { // Will be able to accept other values than float, so change to void*
	GLCall(glGenBuffers(1, &m_Renderer_id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
}

unsigned int VertexBuffer::get_id() const {
	return m_Renderer_id;
}

void VertexBuffer::bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_id));
}

void VertexBuffer::unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_Renderer_id));
}