#pragma once
#include "Box.h"
#include <glut.h>
#include "Vector3.h"
#include "Input.h"
#include "MyModel.h"
#include "Pig.h"
#include "Cow.h"
#include "Steve.h"
#include "NPC.h"
#include "Model.h"

#define PI 3.1415

//Player Character inherits from Non Player Character
class PC : public NPC
{
public:
	//Passed a pointer to the Input class, it's skin and the imported models.
	PC(Input* input_, int skin_, Model models_[3]);
	~PC();

	//Getters
	float getAngle();
	Vector3 getPosition();
	MyModel* getModel();

	//Setters
	void setAngle(float angle_);
	void setPerspective(int perspective);

	//Artificial intelligence. This is not needed for the PC
	virtual void AI(float dt);

	void handleInput(float dt);

	//Pointer to the imported models it is handed.
	Model* models;

private:
	//Which persective the camera is in. Used to tell the render() class whether to render the character or not.
	int perspective_;

	//Which imported model (tool) the player has equipped.
	int toolIndex = 2;

};

