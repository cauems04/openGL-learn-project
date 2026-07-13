#pragma once

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray {
private:
	unsigned int m_Renderer_id;
	const VertexBuffer* m_Vertex_buffer = nullptr;
	const IndexBuffer* m_Index_buffer = nullptr;

public:
	VertexArray();
	~VertexArray();

	void bind() const;
	//void unbind() const;

	void addBuffer(const VertexBuffer* VBO);
	void addIndexes(const IndexBuffer* EBO);
};