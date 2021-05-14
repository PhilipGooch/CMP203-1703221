#pragma once
#include "Box.h"
#include <glut.h>
#include "Vector3.h"
#include "Input.h"
#include "MyModel.h"
#include "Pig.h"
#include "Cow.h"
#include "Steve.h"
#include "Model.h"
#include "Candle.h"

#define PI 3.1415f

//Non Player Character
class NPC
{
public:
	NPC(Input* input_, int skin_, Model models[3]);
	~NPC();

	//Getters
	float getAngle();
	Vector3 getPosition();
	MyModel* getModel();

	//Setters
	void setAngle(float angle_);
	void setPosition(float x, float y, float z);
	virtual void setPerspective(int pointOfView_);
	void setSpawnPosition();

	//These functions pass things straight down to the MyModel class... I should maybe make MyModel inherit from NPC... 
	void setLightPosition(float x, float y, float z);
	void setNight(bool night_);
	void setStencil(std::vector<float>* stencil);
	void setHat(bool hat_);

	virtual void handleInput(float dt);

	void update(float dt);

	virtual void render();
	//Overloaded function for candle, passing it which light to use.
	virtual void render(int light);
	virtual void renderWithShadow();

	//Artificial intelligence for the NPCs (Very stupid artificial intelligence!)
	virtual void AI(float dt);

protected:
	Input * input;
	Vector3 position;
	Vector3 spawnPosition;
	Vector3 velocity;
	Vector3 acceleration;
	//The angle the player is facing. (Have to minus 90 degrees to face forward.)
	float angle;		
	float speed;
	float rotateSpeed;
	//Pointer to the custom model I am using for the NPC.
	MyModel* model;
	//Stores which skin The model will have.
	int skin;
	bool walking;
	//How far the NPC can roam before it goes out of bounds and turns around.
	float roamRadius;
	//Vector used to calculate the angle it needs to turn to walk back to the center of it's roaming circle.
	Vector3 AInormal;

	//For the heading of velocity vector.			
	float direction;							
	//The direction the player is oriented while strafing.
	float strafeDirection;	
};

