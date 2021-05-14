#include "Camera.h"



Camera::Camera(Input* input_, float width_, float height_, int floor)
{
	input = input_;
	width = width_;
	height = height_;
	floor_ = floor;
	zoomSpeed = 0.3;
	minZoom = 0.1;
	rotateSpeed = 2;
	maxZoom = 50;
	deltaMouseX = 0;
	deltaMouseY = 0;
	roll_ = 0;
	zoom = 12;
	setUp(0, 1, 0);
}


Camera::~Camera()
{
}

void Camera::handleInput(float dt)
{
	//Zoom
	if (input->isKeyDown('x')) {
		//input->SetKeyUp('x');
		zoomIn();
	}
	if (input->isKeyDown('z')) {
		//input->SetKeyUp('z');
		zoomOut();
	}

	if (perspective_ == 2) {
		if (input->isKeyDown('w')) {
			moveForward(dt);
		}
		if (input->isKeyDown('s')) {
			moveBackward(dt);
		}
		if (input->isKeyDown('a')) {
			moveLeft(dt);
		}
		if (input->isKeyDown('d')) {
			moveRight(dt);
		}
		if (input->isKeyDown('q')) {
			moveDown(dt);
		}
		if (input->isKeyDown('e')) {
			moveUp(dt);
		}
	}
}

void Camera::update(float dt)
{
	//Calculating the amount the mouse has moved since the last frame.
	deltaMouseX = (input->getMouseX() - width / 2) * rotateSpeed;
	deltaMouseY = (input->getMouseY() - height / 2) * rotateSpeed;

	float cosR, cosP, cosY;
	float sinR, sinP, sinY;

	cosY = cosf(yaw_*3.1415 / 180);
	cosP = cosf(pitch_*3.1415 / 180);
	cosR = cosf(roll_*3.1415 / 180);
	sinY = sinf(yaw_*3.1415 / 180);
	sinP = sinf(pitch_*3.1415 / 180);
	sinR = sinf(roll_*3.1415 / 180);

	forward.setX(sinY * cosP);
	forward.setY(sinP);
	forward.setZ(cosP * -cosY);

	right = forward.cross(up);

	//First person
	if (perspective_ == 0) {
		up.setX(-cosY * sinR - sinY * sinP * cosR);			
		up.setY(cosP * cosR);
		up.setZ(-sinY * sinR - sinP * cosR * -cosY);
		lookAt = position + forward;

		rotateY(-deltaMouseX, dt);
		rotateX(-deltaMouseY, dt);
	}
	//Third person
	else if (perspective_ == 1) {
		//This uses the parametric equation of a sphere.
		position.setX(lookAt.getX() + (sinf(yaw_ * PI / 180) * cosf(pitch_ * PI / 180)) * zoom);
		//Handling when when the camera hits the floor.	(Tweaked, hard-coded variables.)
		if (pitch_ < -5)																	
			position.setY(floor_ + 0.1);			
		else
			position.setY(lookAt.getY() + sinf(pitch_ * PI / 180) * zoom);
		position.setZ(lookAt.getZ() - (cosf(pitch_ * PI / 180) * -cosf(yaw_ * PI / 180)) * zoom);
		//							^ Changed to a minus for 3rd person.
		rotateY(deltaMouseX, dt);
		rotateX(deltaMouseY, dt);
	}
	//Creative mode
	else if (perspective_ == 2) {
		//Setting up to a vector in the y direction.
		up.setX(0);
		up.setY(1);
		up.setZ(0);
		lookAt = position + forward;
		rotateY(-deltaMouseX, dt);
		rotateX(-deltaMouseY, dt);
	}
}

Vector3 Camera::getUp() { return up; }

Vector3 Camera::getPosition() { return position; }

Vector3 Camera::getLookAt() { return lookAt; }

Vector3 Camera::getForward() { return forward; }

float Camera::getYaw() { return yaw_; }

float Camera::getPitch() { return pitch_; }

float Camera::getRoll() { return roll_; }

void Camera::rotateX(float theta, float dt)
{
	pitch_ += theta * dt;
	//Claming the pitch of the camera.
	if (pitch_ > 90) {
		pitch_ = 90;
	}
	if (pitch_ < -90) {
		pitch_ = -90;
	}
}

void Camera::rotateY(float theta, float dt)
{
	yaw_ -= theta * dt;
}

void Camera::setLookAt(Vector3& lookAt_) { lookAt = lookAt_; }

void Camera::setPosition(Vector3& position_) { position = position_; }

void Camera::setPosition(float x, float y, float z)
{
	position.setX(x);
	position.setY(y);
	position.setZ(z);
}

void Camera::setUp(float x, float y, float z)
{
	up.setX(x);
	up.setY(y);
	up.setZ(z);
}

void Camera::setYaw(float yaw) { yaw_ = yaw; }

void Camera::zoomIn()
{
	zoom -= zoomSpeed;
	//Claming the zoom.
	if (zoom < minZoom) {
		zoom = minZoom;
	}
}

void Camera::zoomOut()
{
	zoom += zoomSpeed;
	//Claming the zoom.
	if (zoom > maxZoom) {
		zoom = maxZoom;
	}
}

void Camera::moveForward(float dt)
{
	position.add(forward, dt * speed);
}

void Camera::moveBackward(float dt)
{
	position.subtract(forward, dt * speed);
}

void Camera::moveUp(float dt)
{
	position.add(up, dt * speed);
}

void Camera::moveDown(float dt)
{
	position.subtract(up, dt * speed);
}

void Camera::moveLeft(float dt)
{
	position.subtract(right, dt * speed);
}

void Camera::moveRight(float dt)
{
	position.add(right, dt * speed);
}

void Camera::setPerspective(int perspective) { perspective_ = perspective; }

void Camera::reset(int s)
{
	if (perspective_ == 0 || perspective_ == 2) {
		pitch_ = 0;
	}
	else if (perspective_ == 1) {
		pitch_ = 30;
	}
	roll_ = 0;
	yaw_ = 0;
	zoom = 12;
	setUp(0, 1, 0);
}




