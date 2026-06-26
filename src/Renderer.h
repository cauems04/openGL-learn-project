#pragma once
#include <glad/glad.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) { GLClearErrors();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__)); }

void GLClearErrors();

bool GLLogCall(const char* function, const char* file, const int line);
