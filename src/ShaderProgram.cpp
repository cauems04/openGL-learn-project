#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(const char* filePath) {
	const ShaderSource shaderSource = parseShader(filePath);
	createShaderProgram(shaderSource);
}

ShaderProgram::~ShaderProgram() {
	if (m_Renderer_id != 0) {
		GLCall(glDeleteProgram(m_Renderer_id));
	}
}

void ShaderProgram::use() const {
	GLCall(glUseProgram(m_Renderer_id));
}

unsigned int ShaderProgram::getId() const {
	return m_Renderer_id;
};

void ShaderProgram::createShaderProgram(const ShaderSource& shaderSource) {

	unsigned int shaderProgram = glCreateProgram();

	compileShader(shaderProgram, GL_VERTEX_SHADER, shaderSource.vertexShader.c_str());
	compileShader(shaderProgram, GL_FRAGMENT_SHADER, shaderSource.fragmentShader.c_str());

	GLCall(glLinkProgram(shaderProgram));

	m_Renderer_id = shaderProgram;
}

void ShaderProgram::compileShader(unsigned int shaderProgram, GLenum type, const char* shaderSource) const {

	unsigned int shader = glCreateShader(type);
	const char* source = shaderSource;
	GLCall(glShaderSource(shader, 1, &source, NULL));
	GLCall(glCompileShader(shader));

	GLCall(glAttachShader(shaderProgram, shader));

	GLCall(glDeleteShader(shader));
}

ShaderSource ShaderProgram::parseShader(const char* filePath) const {

	enum ReadShaderType { noneType = -1, vertexType, fragmentType };

	std::ifstream stream(filePath);
	std::string line;

	ReadShaderType readType = noneType;
	std::stringstream ss[2];

	while (getline(stream, line)) {

		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) readType = vertexType;
			else if (line.find("fragment") != std::string::npos) readType = fragmentType;
		}
		else ss[(int)readType] << line << '\n';
	}

	return { ss[0].str(), ss[1].str()};
}

void ShaderProgram::setBool(const char* name, const bool value) const {
	unsigned int uniformLoc = glGetUniformLocation(m_Renderer_id, name);
	GLCall(glUniform1i(uniformLoc, (int)value));
}

void ShaderProgram::setInt(const char* name, const int value) const {
	unsigned int uniformLoc = glGetUniformLocation(m_Renderer_id, name);
	GLCall(glUniform1i(uniformLoc, value));
}

void ShaderProgram::setFLoat(const char* name, const float value) const {
	unsigned int uniformLoc = glGetUniformLocation(m_Renderer_id, name);
	GLCall(glUniform1f(uniformLoc, value));
}

void ShaderProgram::setMat4(const char* name, const glm::mat4 value) const {
	unsigned int uniformLoc = glGetUniformLocation(m_Renderer_id, name);
	GLCall(glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(value)));
}
