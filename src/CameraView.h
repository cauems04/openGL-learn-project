#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class CameraView {
public:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;

	CameraView() = default;
	CameraView(glm::vec3 startPos, glm::vec3 startDir, float startYaw, float startPitch) 
		: cameraPos(startPos), cameraFront(glm::normalize(startDir)), yaw(startYaw), pitch(startPitch) {}

	glm::mat4 getViewMatrix();
	void recalculateCameraFront();
};