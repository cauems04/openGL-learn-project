#pragma once

#include "Renderer.h"

class VertexBuffer {
private:
	unsigned int m_Renderer_id;

public:
	VertexBuffer(const float* vertices, int size); // Will be able to accept other values than float, so change to void*

	unsigned int get_id() const;

	void bind() const;

	void unbind() const;

	~VertexBuffer();
};