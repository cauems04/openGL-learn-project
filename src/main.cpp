#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "stb_image.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

//#define ASSERT(x) if (!(x)) __debugbreak();
//#define GLCall(x) GLClearErrors();\
//	x;\
//	ASSERT(GLLogCall(#x, __FILE__, __LINE__));
//
static struct ShaderSource {
	std::string vertexShader;
	std::string fragmentShader;
};

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

static unsigned int createShaderProgram(std::string vertexShader, std::string fragmentShader);
static ShaderSource parseShader(const char* filePath);

static void processVisibilityChangeInput(GLFWwindow* window, const unsigned int& program, float& visibilityTrade);


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create a window";
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);


	ShaderSource shaderSource = parseShader("res/shaders/Basic.shader");
	unsigned int shaderProgram = createShaderProgram(shaderSource.vertexShader, shaderSource.fragmentShader);

	
	// Read texture image
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* imgData = stbi_load("res/textures/container.jpg", &width, &height, &nrChannels, 0);

	if (!imgData) {
		std::cout << "Failed to load image" << std::endl;
		return 1;
	}

	// Generate, configurate, and set loaded texture image
	unsigned int brickTex;
	glGenTextures(1, &brickTex);
	glBindTexture(GL_TEXTURE_2D, brickTex);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgData);


	imgData = stbi_load("res/textures/awesomeface.png", &width, &height, &nrChannels, 0);

	if (!imgData) {
		std::cout << "Failed to load image" << std::endl;
		return 1;
	}

	// Generate, configurate, and set loaded texture image
	unsigned int happyTex;
	glGenTextures(1, &happyTex);
	glBindTexture(GL_TEXTURE_2D, happyTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgData);


	glBindTexture(GL_TEXTURE_2D, 0);

	
	// Triangle stuff
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom left
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  2.0f, 0.0f, // bottom right
		-0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 2.0f, // top left
		0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 1.0f,  2.0f, 2.0f, // top right
		// positions		 Colors				TexCoordinates
	};

	unsigned int indices[] = {
		2, 0, 1,
		1, 3, 2
	};

	
	VertexArray VAO = VertexArray();
	const VertexBuffer VBO = VertexBuffer(vertices, sizeof(vertices));
	const IndexBuffer EBO = IndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));

	VAO.addBuffer(&VBO);
	VAO.addIndexes(&EBO);
	
	VAO.bind();
	// try to join the attributePointer to the VAO stuff, if related, otherwise, try using it along with the VBO
	// if feels weird being separated from the existent classes (VAOs, VBOs...)
	// Even because if you take off the VAO.bind() just executed, the following attrib pointers will throw errors related to the binded VBO.
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (float*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));

	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (float*)(6 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(2));


	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	

	float visibilityTrade = 0.2;

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		processVisibilityChangeInput(window, shaderProgram, visibilityTrade);

		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUseProgram(shaderProgram));

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, brickTex));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, happyTex));

		GLCall(glUniform1f(glGetUniformLocation(shaderProgram, "visibilityTrade"), visibilityTrade));
		GLCall(glUniform1i(glGetUniformLocation(shaderProgram, "containerTex"), 0));
		GLCall(glUniform1i(glGetUniformLocation(shaderProgram, "happyFaceTex"), 1));

		VAO.bind();
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	glfwTerminate();

	return 0;
}

static void compileShader(unsigned int shaderProgram, GLenum type, const std::string& shaderSource) {

	unsigned int shader = glCreateShader(type);
	const char* source = shaderSource.c_str();
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	glAttachShader(shaderProgram, shader);

	glDeleteShader(shader);
}

static unsigned int createShaderProgram(std::string vertexShader, std::string fragmentShader) {

	unsigned int shaderProgram = glCreateProgram();

	compileShader(shaderProgram, GL_VERTEX_SHADER, vertexShader);
	compileShader(shaderProgram, GL_FRAGMENT_SHADER, fragmentShader);

	glLinkProgram(shaderProgram);

	return shaderProgram;
}


static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
		glfwTerminate();
	}
}

static void processVisibilityChangeInput(GLFWwindow* window, const unsigned int& program, float& visibilityTrade) {
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		
		if (visibilityTrade >= 1) return;

		GLCall(glUniform1f(glGetUniformLocation(program, "visibilityTrade"), visibilityTrade += 0.001f));
	
	} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

		if (visibilityTrade <= 0) return;

		GLCall(glUniform1f(glGetUniformLocation(program, "visibilityTrade"), visibilityTrade -= 0.001f));
	}
}

static ShaderSource parseShader(const char* filePath) {

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

	return { ss[0].str(), ss[1].str() };
}
