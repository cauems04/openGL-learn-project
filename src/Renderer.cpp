#include <iostream>
#include "Renderer.h"

void GLClearErrors() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, const int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << ") " << function << " / " << file << " / Line: " << line << std::endl;
		return false;
	}
	return true;
}