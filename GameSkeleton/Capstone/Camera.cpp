#include "Camera.h"
#include <gtx\transform.hpp>

Camera::Camera() : viewDirection(-0.067f,0.3f,-0.768f), position(0.0f,2.0f,12.0f),UP(0.0f,1.0f,0.0f)
{
	viewDirection=glm::normalize(viewDirection);
	MOVEMENT_SPEED=0.1f;
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position+viewDirection, UP);
}

void Camera::mouseUpdate(const glm::vec2 newMousePosition)
{
	glm::vec2 mouseDelta=newMousePosition-oldMousePosition;

	if(glm::length(mouseDelta) > 10.0f)
	{
		oldMousePosition=newMousePosition;
		return;
	}
	viewDirection=glm::mat3(glm::rotate(-mouseDelta.x, UP)) * viewDirection;
	viewDirection=glm::normalize(viewDirection);

	glm::vec3 toRotateAround=glm::cross(viewDirection, UP);
	viewDirection=glm::mat3(glm::rotate(-mouseDelta.y, toRotateAround)) * viewDirection;
	viewDirection=glm::normalize(viewDirection);

	oldMousePosition=newMousePosition;
}

void Camera::moveForward()
{
	position+=MOVEMENT_SPEED*viewDirection;
}
void Camera::moveBackward()
{
	position -= MOVEMENT_SPEED*viewDirection;
}
void Camera::strafeRight()
{
	glm::vec3 strafeDirection=glm::cross(viewDirection, UP);
	position += MOVEMENT_SPEED*strafeDirection;
}
void Camera::strafeLeft()
{
	glm::vec3 strafeDirection=glm::cross(viewDirection, UP);
	position -= MOVEMENT_SPEED*strafeDirection;
}
void Camera::moveUp()
{
	position+= MOVEMENT_SPEED*UP;
}
void Camera::moveDown()
{
	position -= MOVEMENT_SPEED*UP;
}