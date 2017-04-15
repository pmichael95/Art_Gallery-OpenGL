#include "Camera.h"

// TODO: fiddle around with these values until it feels nice
const float Camera::CAMERA_SPEED = 0.15f;
const float Camera::LOOK_AROUND_SPEED = 0.0025f;

Camera::Camera() {
	PerspectiveCameraParams defaultParams;
	defaultParams.yFov = 45.0f;
	defaultParams.aspectRatio = 1;
	defaultParams.zNearPlane = 0.1f;
	defaultParams.zFarPlane = 100.0f;
	init(defaultParams);
}

Camera::Camera(PerspectiveCameraParams params) {
	init(params);
}

Camera::~Camera() {

}

void Camera::init(PerspectiveCameraParams params) {
	cameraParams = params;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	strafeDirection = glm::cross(viewDirection, upDirection);
}

glm::mat4 Camera::getProjectionMatrix() {
	return glm::perspective(cameraParams.yFov, cameraParams.aspectRatio, cameraParams.zNearPlane, cameraParams.zFarPlane);
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + viewDirection, upDirection);
}

void Camera::setPosition(glm::vec3 position) {
	this->position = position;
}

void Camera::moveForward() {
	position += CAMERA_SPEED * viewDirection;
}

void Camera::moveBackward() {
	position += -CAMERA_SPEED * viewDirection;
}

void Camera::moveLeft() {
	position += -CAMERA_SPEED * strafeDirection;
}

void Camera::moveRight() {
	position += CAMERA_SPEED * strafeDirection;
}
void Camera::moveUp() {
	position += CAMERA_SPEED * upDirection;
}

void Camera::moveDown() {
	position += -CAMERA_SPEED * upDirection;
}

void Camera::updateMouse(glm::vec2& newPos) {
	glm::vec2 mouseDelta = newPos - oldMousePos;
	strafeDirection = glm::cross(viewDirection, upDirection);
	glm::mat4 rotationXY = glm::rotate(glm::mat4(), -mouseDelta.x * LOOK_AROUND_SPEED, upDirection) * glm::rotate(glm::mat4(), -mouseDelta.y * LOOK_AROUND_SPEED, strafeDirection);
	viewDirection = glm::mat3(rotationXY) * viewDirection;
	oldMousePos = newPos;
}

const glm::vec3& Camera::getCameraPosition() const {
	return position;
}
