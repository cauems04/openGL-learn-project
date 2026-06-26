#pragma once

#include "Renderer.h"

class IndexBuffer {
private:
	unsigned int m_Renderer_id;
	int m_Count;
public:

	IndexBuffer(const unsigned int* vertices, const int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	int getCount() const;
};