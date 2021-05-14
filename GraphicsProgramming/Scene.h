// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).
#ifndef _SCENE_H
#define _SCENE_H

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
#include <cassert>
#include "Input.h"
#include "SOIL.h"
#include "Camera.h"
#include "Block.h"
#include "TextureLoader.h"
#include "NPC.h"
#include "PC.h"
#include "MyModel.h"
#include "Cow.h"
#include "Pig.h"
#include "Steve.h"
#include "Skybox.h"
#include "Model.h"
#include "Shadow.h"
#include "Rainbow.h"

//Stores the position of a block space.			
struct BlockSpace {
	int x, y, z;
};

class Scene{
public:
	Scene(Input *in);
	~Scene();
	// Main render function
	void render();
	// Handle input function that receives delta time from parent.
	void handleInput(float dt);
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);

protected:
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	void calculateFPS();

	// For access to user input.
	Input* input;
		
	// For Window and frustum calculation.
	int width, height;
	float fov, nearPlane, farPlane;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char text[40];

	//Camera
	Camera* camera;
	float deltaMouseX;
	float deltaMouseY;

	//Textures
	TextureLoader textureLoader;
	GLuint* textures[12][6];

	//Size of the grid.
	static const int s = 32;

	//Used to indicate which camera to use.
	int perspective;

	//Loading and saving the map.
	void load();
	void save();

	//Used to load the map.
	std::string fileMap[s][s];

	//Multiple ways of storing the blocks.		//This could be optimised... I don't need 3 arrays.
	int grid[s][s][s] = { 0 };				
	Block* blocks[s][s][s];
	BlockSpace blockSpaces[s][s][s];

	//Used to pass to my blocks' render() function to determine which faces get rendered.
	bool visibleFaces[6];
	void calculateVisibleFaces(int x, int y, int z, int mirrored);

	//Ray casting
	//The block that the ray has intersected with.
	BlockSpace* intersectionBlock = nullptr;

	//currentBlockSpace refers to the block space the ray casting algorithm is looking at. This always starts at the block space the camera is in.
	BlockSpace* currentBlockSpace;

	//This is the blockspace the camera is in.
	BlockSpace* cameraBlockSpace;

	//This is the block that is being targeted.
	Block* targetedBlock;

	//This is the block the ray has intersected.				//targetedBlock and collisionBlock are the same...! Consider merging... 
	Block* collisionBlock = nullptr;

	//The point of intersection of the line and the given plane.
	float pointOfIntersection[3];

	//This keeps trace of the face that has already been intersected to get to the current block space. -1 indicates it is at the origin. (The block space the camera is in.)
	int intersectedFace = -1;

	//This is the face that has been intersected of the block that has been detected.
	int targetedFace;

	//How many blocks the ray can go through before the algorithm stops.
	int rayMaxDistance = 7;
	int rayDistance = 0;
	bool rayCast(BlockSpace* currentBlockSpace);
	void calculatePointOfIntersection(BlockSpace* currentBlockSpace, int face);
	BlockSpace* intersection(BlockSpace* currentBlockSpace, int face);
	Block* collision(BlockSpace* currentBlockSpace, int face);

	//Variables used for the intersection of lines and planes maths.
	float normal[3];
	float pointOnPlane[3];
	float d;
	float t;

	//Creating my NPC and PC characters.
	const int numberOfCows = 1;
	const int numberOfPigs = 1;
	NPC* steve;
	NPC* cows[1];
	NPC* pigs[1];
	NPC* candle1;				//I added the candle last minute and making it an NPC was the quickest way I could think of doing it...!
	NPC* candle2;
	NPC* candle3;

	//Rainbow
	Rainbow* rainbow;
	GLuint rainbowTexture;

	//Skybox 
	Skybox* skybox;
	GLuint skyBoxDayTexture;
	GLuint skyBoxNightTexture;

	//Used for lighting position and for shadow calculations.
	float sunAngle = 40;
	float sunSpeed = 0.01f;

	//Variables for messing with the light intensity of the moon.
	float moonD = 0.5f;
	float moonA = 0.35f;

	//Imported models.
	Model models[3];
	int modelIndex = 0;

	//Stencils
	std::vector<float> shadowStencil;
	std::vector<float> reflectionStencil;
	void generateShadowStencil();
	void generateReflectionStencil();

	bool night = true;
	bool hat = false;
	bool displayIndicator = false;
	bool flashLight = false;
	int selectedBuildingBlock = 1;

	//Broken... :(
	//void mirror(Block* block, int face);				//Would orefer this to be a class but i couldnt pass the blocks array to it...

	//Not used...
	//Bloolean for checking if the block is concealed on all sides by other blocks.
	//bool concealed();		

	bool start = false;
};

#endif