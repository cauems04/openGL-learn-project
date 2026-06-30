#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

#include "Renderer.h"
#include "glm/gtc/type_ptr.hpp"

struct ShaderSource {
	const std::string vertexShader;
	const std::string fragmentShader;
};

class ShaderProgram {
private:
	unsigned int m_Renderer_id = 0;

public:
	ShaderProgram(const char* filePath);
	~ShaderProgram();

	void use() const;
	unsigned int getId() const;

	//Uniforms
	void setInt(const char* name, const int value) const;
	void setFLoat(const char* name, const float value) const;
	void setBool(const char* name, const bool value) const;
	void setMat4(const char* name, const glm::mat4 value) const;

private:
	void createShaderProgram(const ShaderSource& shaderSource);
	void compileShader(unsigned int shaderProgram, GLenum type, const char* shaderSource) const;
	ShaderSource parseShader(const char* filePath) const;
};