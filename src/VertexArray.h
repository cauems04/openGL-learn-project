#pragma once

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray {
private:
	unsigned int m_Renderer_id;
	const VertexBuffer* m_Vertex_buffer;
	const IndexBuffer* m_Index_buffer;

public:
	VertexArray();
	~VertexArray();

	void bind() const;
	//void unbind() const;

	void addBuffer(const VertexBuffer* VBO);
	void addIndexes(const IndexBuffer* EBO);
};