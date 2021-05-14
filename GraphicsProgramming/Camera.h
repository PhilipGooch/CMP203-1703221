#pragma once

#include "Vector3.h"
#include <map>
#include <string>
#include "Input.h"
#include <math.h>

#define PI 3.1415

class Camera
{
public:
	Camera(Input* input_, float width_, float height_, int floor);
	~Camera();
	void handleInput(float dt);
	void update(float dt);

	//Getters
	Vector3 getUp();
	Vector3 getForward();
	Vector3 getPosition();
	Vector3 getLookAt();
	float getYaw();
	float getPitch();
	float getRoll();

	//Setters
	void setLookAt(Vector3& lookAt_);
	void setPosition(Vector3& position_);
	void setPosition(float x, float y, float z);
	void setUp(float x, float y, float z);
	void setYaw(float yaw);
	void setPerspective(int perspective_);

	//Movement functions for flying camera.
	void moveForward(float dt);
	void moveBackward(float dt);
	void moveUp(float dt);
	void moveDown(float dt);
	void moveLeft(float dt);
	void moveRight(float dt);

	//Rotation functions that are called from scene, passing them deltaMouse x and y.
	void rotateX(float theta, float dt);
	void rotateY(float theta, float dt);

	//Zoom functions.
	void zoomIn();
	void zoomOut();

	//Reseting the camera's position and orientation. Takes the size of the block grid as a parameter.
	void reset(int s);

private:
	Input* input;

	//Width and height of the viewport.
	float width, height;				

	//The amount the mouse has moved since the last frame.
	float deltaMouseX, deltaMouseY;

	float yaw_;
	float pitch_;
	float roll_;
	float zoom;
	Vector3 forward;
	Vector3 up;
	Vector3 right;					
	Vector3 position;
	Vector3 lookAt;
	float rotateSpeed, zoomSpeed;
	float minZoom, maxZoom;

	//Which camera mode it is in.
	int perspective_ = 1;

	//The top level of the environment.
	int floor_;

	//The speed of the camera in flying mode.
	float speed = 0.5f;
};

