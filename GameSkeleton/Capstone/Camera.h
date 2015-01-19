#pragma once
#include <glm.hpp>

class Camera
{
	const glm::vec3 UP;
	glm::vec2 oldMousePosition;
public:
	glm::vec3 viewDirection;
	glm::vec3 position;
	Camera();
	glm::mat4 getWorldToViewMatrix() const;

	float MOVEMENT_SPEED;

	void moveForward();
	void moveBackward();
	void strafeRight();
	void strafeLeft();
	void moveUp();
	void moveDown();
	void mouseUpdate(const glm::vec2);
};