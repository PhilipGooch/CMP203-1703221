#pragma once
#include "Input.h"
#include "Box.h"
#include <glut.h>
#include "Vector3.h"
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Shadow.h"

class MyModel		//Maybe model should inherit from NPC...?
{
public:
	MyModel();
	~MyModel();
	void animate(float dt);
	void resetAnimation();
	virtual void render();
	//Overloaded function for candle, passing it which light to use.
	virtual void render(int light);
	virtual void renderWithShadow();

	//Getters for the cuboids.
	Box* getBody();				//these want to be in an array...? it is starting to share a lot of variables with it...
	Box* getHead();
	Box* getLeftArm();
	Box* getRightArm();
	Box* getLeftLeg();
	Box* getRightLeg();
	Box* getNose();
	Box* getLeftHorn();
	Box* getRightHorn();
	Box* getUdders();

	//Getters for the Vector offsets. Passed by reference.
	Vector3& getBodyOffset();
	Vector3& getHeadOffset();
	Vector3& getLeftArmOffset();
	Vector3& getRightArmOffset();
	Vector3& getLeftLegOffset();
	Vector3& getRightLegOffset();
	Vector3& getNoseOffset();
	Vector3& getLeftHornOffset();
	Vector3& getRightHornOffset();
	Vector3& getUddersOffset();

	//Returns a float for the models eye position. Used for first person camera.
	float getEyesY();

	//Returns a pointer to the limb angles array.
	float* getlimbAngles();

	//Setters
	void setToolIndex(int modelIndex_);
	void setHat(bool hat_);
	void setPosition(float x, float y, float z);		
	void setAngle(float angle_);
	void setDirection(float direction_);
	void setStrafeDirection(float strafeDirection_);
	void setLightPosition(float x, float y, float z);
	void setNight(bool night_);
	void setStencil(std::vector<float>* stencil);


protected:
	Input * input;

	//Individual cuboids.
	Box* body;				//array of cuboids...?
	Box* head;
	Box* leftArm;
	Box* rightArm;
	Box* leftLeg;
	Box* rightLeg;
	Box* nose;
	Box* leftHorn;
	Box* rightHorn;
	Box* udders;

	//Cuboid offests.
	Vector3 bodyOffset;			//Also an array...?
	Vector3 headOffset;
	Vector3 leftArmOffset;
	Vector3 rightArmOffset;
	Vector3 leftLegOffset;
	Vector3 rightLegOffset;
	Vector3 noseOffset;
	Vector3 leftHornOffset;
	Vector3 rightHornOffset;
	Vector3 uddersOffset;

	//Used for rotating the limbs from the right place.
	Vector3 limbAnimationTranslate;

	//y position of the eyes.
	float eyesY;

	//Storing the textures.
	GLuint textures[9];

	//Limb animation variables.
	int limbDirections[4];
	float limbAngles[4];
	float limbSpeed;

	Vector3 position;							//No need to have duplicate values in this class... It should inherit from NPC...
	float size;
	//The angle the model is facing. (Have to minus 90 degrees to face forward.)
	float angle;				
	float speed;
	//The heading of velocity vector.
	float direction;			
	//The direction the player is oriented while strafing.
	float strafeDirection;		

	bool night = true;
	bool hat = false;
	int toolIndex;

	//Shadow.
	Shadow shadow;
	//Storing stencil vectors for shadows.
	std::vector<float>* stencil;
	//Variables used for creating the shadow.
	GLfloat lightPosition[4];
	float shadowMatrix[16];
	GLfloat floorVerts[12] = {
		0, 14.01f, 0,	//top left
		0, 14.01f, 32,	//bottom left
		32, 14.01f, 32,	//bottom right
		32, 14.01f, 0,	//top right
	};
};

