#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Renderer.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "CameraView.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window, CameraView& cameraView);
static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

double lastX = 00; double lastY = 00;
static bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

float fov = 45.0f;

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	CameraView cameraView = CameraView();
	glfwSetWindowUserPointer(window, &cameraView);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);

	// Textures
	unsigned int diffTexture;
	{
		int width, height, nrChannels;
		glGenTextures(1, &diffTexture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffTexture);

		unsigned char* data = stbi_load("res/textures/container2.png", &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	unsigned int specTexture;
	{
		int width, height, nrChannels;
		glGenTextures(1, &specTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specTexture);

		unsigned char* data = stbi_load("res/textures/container2_specular.png", &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}



	ShaderProgram shaderProgram = ShaderProgram("Basic.shader");
	ShaderProgram lightCubeShaderProgram = ShaderProgram("LightCube.shader");
	
	// Triangle stuff
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	VertexArray VAO = VertexArray();
	const VertexBuffer VBO = VertexBuffer(vertices, sizeof(vertices));
	//const IndexBuffer EBO = IndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));
	VAO.bind();
	VAO.addBuffer(&VBO);
	//VAO.addIndexes(&EBO);
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));

	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(2));
	// try to join the attributePointer to the VAO stuff, if related, otherwise, try using it along with the VBO. If feels weird being separated from the existent classes (VAOs, VBOs...). Even because if you take off the VAO.bind() just executed, the following attrib pointers will throw errors related to the binded VBO


	VertexArray lightVAO = VertexArray();
	lightVAO.bind();
	lightVAO.addBuffer(&VBO);
	//VAO.addIndexes(&EBO);
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	// Model translation vectors
	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightSourcePos = glm::vec3(1.2f, 1.0f, 2.0f);

	shaderProgram.use();
	shaderProgram.setInt("material.diffuse", 0);
	shaderProgram.setInt("material.specular", 1);
	shaderProgram.setFloat("material.intensity", 32.0f);


	shaderProgram.setVec3("dirLight.ambient", lightColor * 0.1f);
	shaderProgram.setVec3("dirLight.diffuse", lightColor);
	shaderProgram.setVec3("dirLight.specular", lightColor * 0.8f);

	//lightCubeShaderProgram.use();

	while (!glfwWindowShouldClose(window)) {
		processInput(window, cameraView);
		
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightSourcePos.x = 5.0f * (float)glm::sin(glm::radians(glfwGetTime() * 50.0f));

		VAO.bind();

		glm::mat4 view = cameraView.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		
		shaderProgram.use();
		shaderProgram.setVec3("dirLight.direction", glm::vec3(view * glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)));
		//shaderProgram.setVec3("light.position", glm::vec3(view * glm::vec4(lightSourcePos, 1.0f)));

		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);

		for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			shaderProgram.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightVAO.bind();
		glm::mat4 lightCubeModel = glm::mat4(1.0f);
		lightCubeModel = glm::translate(lightCubeModel, lightSourcePos);
		lightCubeModel = glm::scale(lightCubeModel, glm::vec3(0.2f, 0.2f, 0.2f));
		
		lightCubeShaderProgram.use();
		lightCubeShaderProgram.setMat4("model", lightCubeModel);
		lightCubeShaderProgram.setMat4("view", view);
		lightCubeShaderProgram.setMat4("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window, CameraView& cameraView) {

	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
	}
	else if (glfwGetKey(window, GLFW_KEY_W)) {
		cameraView.cameraPos += cameraSpeed * cameraView.cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_S)) {
		cameraView.cameraPos -= cameraSpeed * cameraView.cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_A)) {
		cameraView.cameraPos -= glm::normalize(glm::cross(cameraView.cameraFront, cameraView.cameraUp)) * cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_D)) {
		cameraView.cameraPos += glm::normalize(glm::cross(cameraView.cameraFront, cameraView.cameraUp)) * cameraSpeed;
	}
}

static void mouseCallback(GLFWwindow* window, double xpos, double ypos){
	CameraView* cameraView = static_cast<CameraView*>(glfwGetWindowUserPointer(window));
	
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos; 

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	float* yaw = &cameraView->yaw;
	float* pitch = &cameraView->pitch;

	*yaw += xoffset;
	*pitch += yoffset;

	if (*pitch < -89.9f) {
		*pitch = -89.9;
	} else if (*pitch > 89.9f) {
		*pitch = 89.9;
	}
	
	cameraView->recalculateCameraFront();
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= (float)yoffset;
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	else if (fov > 45.0f) {
		fov = 45.0f;
	}
};
