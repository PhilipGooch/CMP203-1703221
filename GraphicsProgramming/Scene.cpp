#include "Scene.h"
#include <math.h>
#include <iostream>

#define PI 3.1415926535897

Scene::Scene(Input *in)
{
	//Storing a pointer to instance of input class.
	input = in;

	//Width and height of window.
	width = 1300;
	height = 700;	
		
	glShadeModel(GL_SMOOTH);							// Enable smooth shading
	glClearColor(1, 1, 1, 1);							// Setting background color
	glClearDepth(1.0f);									// Depth suffer setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables depth testing
	glDepthFunc(GL_LEQUAL);								// The type of depth testing to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations
	glEnable(GL_BLEND);									// Enabling blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Blending setup
	glEnable(GL_LIGHTING);								// Enabling lighting
	glutSetCursor(GLUT_CURSOR_NONE);					// Hiding mouse cursor
	glEnable(GL_TEXTURE_2D);							// Enabling texture2D
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	// Texture setup
	glPolygonMode(GL_FRONT, GL_FILL);					// Not wireframe mode

	//Creating camera. Passing it a pointer to the input object, the width and height of the window and the top level of the geometry.
	camera = new Camera(input, width, height, 14);
	
	//Creating my custom models for steve, pigs and cows. Passing them a pointer to the input object, the skin it will have and the array of models (tools) they could be carrying.
	steve = new PC(input, 0, models);
	for (int i = 0; i < numberOfCows; i++) {
		cows[i] = new NPC(input, 2, models);
	}
	for (int i = 0; i < numberOfPigs; i++) {
		pigs[i] = new NPC(input, 1, models);
	}
	candle1 = new NPC(input, 3, models);		//I added the candle last and I was in a hurry so I used the same code as for my NPCs and just made it static...!
	candle2 = new NPC(input, 3, models);
	candle3 = new NPC(input, 3, models);

	//Setting the position of my custom models.
	steve->setPosition(16.000001, 14.000001, 16.000001);		//.0000001 for offsetting the player to fix ray casting bug...
	for (int i = 0; i < numberOfCows; i++) {
		cows[i]->setPosition(16 + rand() % 4 - 2, 14, 16 + rand() % 4 - 2);
		cows[i]->setSpawnPosition();
	}
	for (int i = 0; i < numberOfPigs; i++) {
		pigs[i]->setPosition(16, 14, 16);
		pigs[i]->setSpawnPosition();
	}
	candle1->setPosition(22, 14, 15);
	candle1->setSpawnPosition();
	candle2->setPosition(24, 14, 14);
	candle2->setSpawnPosition();
	candle3->setPosition(20, 14, 15);
	candle3->setSpawnPosition();

	//Loading textures for skyboxs.
	skyBoxDayTexture = SOIL_load_OGL_texture
	(
		"gfx/skyboxDay.png",
		//"gfx/skyboxTiny.png",			//1x1 image for loading time.
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	skyBoxNightTexture = SOIL_load_OGL_texture
	(
		"gfx/skyboxNight.png",
		//"gfx/skyboxTiny.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	//Creating skybox.
	skybox = new Skybox(&skyBoxDayTexture, &skyBoxNightTexture);

	//Populating my textures array using my TextureLoader class.
	for (int i = 0; i < 6; i++) textures[0][i] = nullptr;												//Is it ok that textureLoader GLuints are public...?		
	for (int i = 0; i < 4; i++) textures[1][i] = &textureLoader.grassSide;			//	#1 = grass
								textures[1][4] = &textureLoader.grassTop;		
								textures[1][5] = &textureLoader.earth;			
	for (int i = 0; i < 6; i++)	textures[2][i] = &textureLoader.sand;				//	#2 = earth
	for (int i = 0; i < 6; i++)	textures[3][i] = &textureLoader.earth;				//	#3 = sand
	for (int i = 0; i < 6; i++)	textures[4][i] = &textureLoader.gravel;				//	#4 = gravel
	for (int i = 0; i < 6; i++)	textures[5][i] = &textureLoader.stone;				//	#5 = stone
	for (int i = 0; i < 4; i++)	textures[6][i] = &textureLoader.woodSide;			//	#6 = wood;
	for (int i = 4; i < 6; i++)	textures[6][i] = &textureLoader.woodTop;
	for (int i = 0; i < 6; i++)	textures[7][i] = &textureLoader.leaves;				//	#7 = leaves
	for (int i = 0; i < 6; i++)	textures[8][i] = &textureLoader.redStoneSparkle;	//	#8 = redstone
	for (int i = 0; i < 6; i++)	textures[9][i] = &textureLoader.water;				//	#9 = water
	for (int i = 0; i < 6; i++)	textures[10][i] = &textureLoader.redStoneShell;		

	//Loading the map from a txt file.
	load();

	//Creating block spaces. Blockspace is a struct that simply holds its grid position.
	for (int y = 0; y < s; y++) {
		for (int z = 0; z < s; z++) {
			for (int x = 0; x < s; x++) {
				blockSpaces[y][z][x].x = x;
				blockSpaces[y][z][x].y = y;
				blockSpaces[y][z][x].z = z;
			}
		}
	}

	//Creating blocks from grid array.
	for (int y = 0; y < s; y++) {
		for (int z = 0; z < s; z++) {
			for (int x = 0; x < s; x++) {
				if (grid[y][z][x] == 0) {
					blocks[y][z][x] = nullptr;
				}
				else if (grid[y][z][x] == 8) {
					//Passing the block its textures, its type and its position.
					blocks[y][z][x] = new Block(textures[grid[y][z][x]], textures[10], grid[y][z][x], Vector3((float)x, (float)y, (float)z));
				}
				else {
					blocks[y][z][x] = new Block(textures[grid[y][z][x]], grid[y][z][x], Vector3((float)x, (float)y, (float)z));
				}
			}
		}
	}

	//Loading models from obj files.
	models[0].load("models/diamondAxe.obj", "gfx/diamondAxe.png");
	models[1].load("models/diamondPickaxe.obj", "gfx/diamondPickAxe.png");
	models[2].load("models/diamondSword.obj", "gfx/diamondSword.png");

	//Setting the perspective of the player. 
	// 0 = first person.
	// 1 = third person.
	// 2 = creative mode.
	perspective = 1;
	camera->setPerspective(perspective);
	steve->setPerspective(perspective);
	//Setting the camera position and orientation.
	camera->reset(s);

	//Creating a stencil of the top layer of my geometry. This is used for shadows.
	generateShadowStencil();			

	//Loading rainbow texture.
	rainbowTexture = SOIL_load_OGL_texture
	(
		"gfx/rainbow.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	//Creating the rainbow. Passing it its texture, the number of segments it will have, its radius and its width.
	rainbow = new Rainbow(&rainbowTexture, 20, 5);

	//Creating a stencil of any water blocks on the top level. This is used for reflection.
	generateReflectionStencil();
}

Scene::~Scene()
{
	//Deleting objects created on the the heap.
	for (int y = 0; y < s; y++) {
		for (int z = 0; z < s; z++) {
			for (int x = 0; x < s; x++) {
				delete blocks[y][z][x];
			}
		}
	}
	delete rainbow;
}

void Scene::handleInput(float dt)
{
	steve->handleInput(dt);
	camera->handleInput(dt);

	//Wireframe mode.
	if (input->isKeyDown('r')) {
		input->SetKeyUp('r');
		skybox->switchWireFrame();
		for (int y = 0; y < s; y++) {
			for (int z = 0; z < s; z++) {
				for (int x = 0; x < s; x++) {
					if (blocks[y][z][x] != nullptr) {
						blocks[y][z][x]->switchWireFrame();
					}
				}
			}
		}
	}
	//Placing blocks.
	if (perspective == 2) {
		if (input->leftMouseButtonPressed() && targetedBlock != nullptr) {
			input->setLeftMouseButton(false);
			int x = targetedBlock->getPosition()->getX();
			int y = targetedBlock->getPosition()->getY();
			int z = targetedBlock->getPosition()->getZ();
			switch (targetedFace) {
			case 0:
				if (z < s - 1 && grid[y][z + 1][x] == 0) {
					//Updating the grid array to the block type.
					grid[y][z + 1][x] = selectedBuildingBlock;
					//Creating a block.
					//If ore block.
					if (selectedBuildingBlock == 8)
						blocks[y][z + 1][x] = new Block(textures[grid[y][z + 1][x]], textures[10], grid[y][z + 1][x], Vector3((float)x, (float)y, (float)z + 1));
					else
						blocks[y][z + 1][x] = new Block(textures[grid[y][z + 1][x]], grid[y][z + 1][x], Vector3((float)x, (float)y, (float)z + 1));
				}
				break;
			case 1:
				if (x < s - 1 && grid[y][z][x + 1] == 0) {
					grid[y][z][x + 1] = selectedBuildingBlock;
					if (selectedBuildingBlock == 8)
						blocks[y][z][x + 1] = new Block(textures[grid[y][z][x + 1]], textures[10], grid[y][z][x + 1], Vector3((float)x + 1, (float)y, (float)z));
					else
						blocks[y][z][x + 1] = new Block(textures[grid[y][z][x + 1]], grid[y][z][x + 1], Vector3((float)x + 1, (float)y, (float)z));
				}
				break;
			case 2:
				if (z > 0 && grid[y][z - 1][x] == 0) {
					grid[y][z - 1][x] = selectedBuildingBlock;
					if (selectedBuildingBlock == 8)
						blocks[y][z - 1][x] = new Block(textures[grid[y][z - 1][x]], textures[10], grid[y][z - 1][x], Vector3((float)x, (float)y, (float)z - 1));
					else
						blocks[y][z - 1][x] = new Block(textures[grid[y][z - 1][x]], grid[y][z - 1][x], Vector3((float)x, (float)y, (float)z - 1));
				}
				break;
			case 3:
				if (x > 0 && grid[y][z][x - 1] == 0) {
					grid[y][z][x - 1] = selectedBuildingBlock;
					if (selectedBuildingBlock == 8)
						blocks[y][z][x - 1] = new Block(textures[grid[y][z][x - 1]], textures[10], grid[y][z][x - 1], Vector3((float)x - 1, (float)y, (float)z));
					else
						blocks[y][z][x - 1] = new Block(textures[grid[y][z][x - 1]], grid[y][z][x - 1], Vector3((float)x - 1, (float)y, (float)z));
				}
				break;
			case 4:
				if (y < s - 1 && grid[y + 1][z][x] == 0) {
					grid[y + 1][z][x] = selectedBuildingBlock;
					if (selectedBuildingBlock == 8)
						blocks[y + 1][z][x] = new Block(textures[grid[y + 1][z][x]], textures[10], grid[y + 1][z][x], Vector3((float)x, (float)y + 1, (float)z));
					else
						blocks[y + 1][z][x] = new Block(textures[grid[y + 1][z][x]], grid[y + 1][z][x], Vector3((float)x, (float)y + 1, (float)z));
				}
				break;
			case 5:
				if (y > 0 && grid[y - 1][z][x] == 0) {
					grid[y - 1][z][x] = selectedBuildingBlock;
					if (selectedBuildingBlock == 8)
						blocks[y - 1][z][x] = new Block(textures[grid[y - 1][z][x]], textures[10], grid[y - 1][z][x], Vector3((float)x, (float)y - 1, (float)z));
					else
						blocks[y - 1][z][x] = new Block(textures[grid[y - 1][z][x]], grid[y - 1][z][x], Vector3((float)x, (float)y - 1, (float)z));
				}
				break;
			}
		}
		//Deleting blocks.
		if (input->rightMouseButtonPressed() && targetedBlock != nullptr) {
			input->setRightMouseButton(false);
			int x = targetedBlock->getPosition()->getX();
			int y = targetedBlock->getPosition()->getY();
			int z = targetedBlock->getPosition()->getZ();
			//If the block space has something in it, delete it!		//I am using grid and blockspace that do similar things... I should deprecate one... Blocks also knows its position. I could maybe just use that...?
			if (grid[y][z][x] != 0) {
				grid[y][z][x] = 0;
				blocks[y][z][x] = nullptr;
			}
			if (y == 13) {
				if (grid[y][z][x] == 9) {
					//If the block is a water block, recalculate the reflection stencil.
					generateReflectionStencil();
				}
				generateShadowStencil();
			}
		}
	}
	//Reseting camera.
	if (input->isKeyDown('v')) {
		input->SetKeyUp('v');
		camera->reset(s);
	}
	//Break point.
	if (input->isKeyDown('g')) {
		input->SetKeyUp('g');
		int i = 0;
	}
	//Save.
	if (input->isKeyDown('h')) {
		input->SetKeyUp('h');
		save();
	}
	//Selecing building blocks.
	if (input->isKeyDown('1')) {
		input->SetKeyUp('1');
		selectedBuildingBlock = 1;
	}
	else if (input->isKeyDown('2')) {
		input->SetKeyUp('2');
		selectedBuildingBlock = 2;
	}
	else if (input->isKeyDown('3')) {
		input->SetKeyUp('3');
		selectedBuildingBlock = 3;
	}
	else if (input->isKeyDown('4')) {
		input->SetKeyUp('4');
		selectedBuildingBlock = 4;
	}
	else if (input->isKeyDown('5')) {
		input->SetKeyUp('5');
		selectedBuildingBlock = 5;
	}
	else if (input->isKeyDown('6')) {
		input->SetKeyUp('6');
		selectedBuildingBlock = 6;
	}
	else if (input->isKeyDown('7')) {
		input->SetKeyUp('7');
		selectedBuildingBlock = 7;
	}
	else if (input->isKeyDown('8')) {
		input->SetKeyUp('8');
		selectedBuildingBlock = 8;
	}
	else if (input->isKeyDown('9')) {
		input->SetKeyUp('9');
		selectedBuildingBlock = 9;
		generateReflectionStencil();
	}
	//Switching day and night.
	if (input->isKeyDown('p')) {
		input->SetKeyUp('p');
		skybox->switchTexture();
		night = !night;
		steve->setNight(night);
		for (int i = 0; i < numberOfCows; i++) {
			cows[i]->setNight(night);
		}
		for (int i = 0; i < numberOfPigs; i++) {
			pigs[i]->setNight(night);
		}
	}
	//Switch perspective								 
	if (input->isKeyDown('b')) {
		input->SetKeyUp('b');
		perspective++;
		if (perspective == 3) {
			perspective = 0;
		}
		//First Person
		if (perspective == 0) {
			camera->setPosition(steve->getPosition().getX(), steve->getPosition().getY() + steve->getModel()->getEyesY(), steve->getPosition().getZ());
			camera->setYaw(steve->getAngle());
		}
		//Third Person
		else if (perspective == 1) {
			camera->setYaw(-camera->getYaw());
		}
		//Creative mode
		else if (perspective == 2) {
			camera->setPosition(steve->getPosition().getX(), steve->getPosition().getY() + steve->getModel()->getEyesY() + 3.f, steve->getPosition().getZ());
			camera->setYaw(-camera->getYaw());
		}
		steve->setPerspective(perspective);
		camera->setPerspective(perspective);
		camera->reset(s);
	}
	//Temporary hotkeys for changing brightness of the moon for testing.
	if (input->isKeyDown('u')) {
		input->SetKeyUp('u');
		moonA += 0.1;
	}
	if (input->isKeyDown('j')) {
		input->SetKeyUp('j');
		moonA -= 0.1;
	}
	if (input->isKeyDown('i')) {
		input->SetKeyUp('i');
		moonD += 0.1;
	}
	if (input->isKeyDown('k')) {
		input->SetKeyUp('k');
		moonD -= 0.1;
	}
	//Switching flashlight on and off.
	if (input->isKeyDown('n')) {
		input->SetKeyUp('n');
		flashLight = !flashLight;
	}
	//Equip hat.
	if (input->isKeyDown('y')) {
		input->SetKeyUp('y');
		hat = !hat;
		steve->setHat(hat);
	}
}

void Scene::update(float dt)
{
	//First person								
	if (perspective == 0) {
		//Setting camera position to Steve's position.
		camera->setPosition(steve->getPosition().getX(), steve->getPosition().getY() + steve->getModel()->getEyesY(), steve->getPosition().getZ());
		//Setting steves angle to the camera's yaw.
		steve->setAngle(camera->getYaw());
	}
	//Third person
	else if (perspective == 1) {
		//Setting camera's lookAt vector to the player.
		camera->setLookAt(steve->getPosition() + steve->getModel()->getBodyOffset());
		//If right click is pressed, set Steve's angle to the negative of the camera's yaw.
		if (input->rightMouseButtonPressed()) {
			steve->setAngle(-camera->getYaw());
		}
	}
	//Creative mode does not use the player's position.

	//Updating the camera.F
	camera->update(dt);

	//Updating my custom models.
	for (int i = 0; i < numberOfCows; i++) {
		cows[i]->update(dt);
	}
	for (int i = 0; i < numberOfPigs; i++) {
		pigs[i]->update(dt);
	}
	steve->update(dt);
	candle1->update(dt);
	candle2->update(dt);
	candle3->update(dt);

	//Forcing the mouse position back to the center of the window.
	glutWarpPointer(width / 2, height / 2);

	//Setting the camera block space to the integer value of the camera's position.
	cameraBlockSpace = &blockSpaces[(int)(camera->getPosition().getY())][(int)(camera->getPosition().getZ())][(int)(camera->getPosition().getX())];

	//Raycasting
	int cameraGridPositionX = (int)(camera->getPosition().getX() + 1);		//Duplicate code...?
	int cameraGridPositionY = (int)(camera->getPosition().getY() + 1);
	int cameraGridPositionZ = (int)(camera->getPosition().getZ() + 1);
	//If the camera is withing the bounds of the grid.
	if (cameraGridPositionX >= 0 && cameraGridPositionX < s - 1 && cameraGridPositionY >= 0 && cameraGridPositionY < s - 1 && cameraGridPositionZ >= 0 && cameraGridPositionZ < s - 1) {
		rayCast(cameraBlockSpace);
	}

	//Incrementing the sun's angle.
	//sunAngle += dt * sunSpeed;

	//Calculating the frames per second.
	calculateFPS();
}

void Scene::render() {
	//Clearing the color, depth and stencil buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//Resetting the transformations.
	glLoadIdentity();
	//Setting the camera's position, lookAt and up vectors.
	gluLookAt(camera->getPosition().getX(), camera->getPosition().getY(), camera->getPosition().getZ(), camera->getLookAt().getX(), camera->getLookAt().getY(), camera->getLookAt().getZ(), camera->getUp().getX(), camera->getUp().getY(), camera->getUp().getZ());

	//Setting up data for lighting calculations.
	GLfloat flashLightDiffuse[] = { 1, 1, 1, 1 };
	GLfloat flashLightAmbient[] = { 0.7, 0.7, 0.7, 1 };
	GLfloat flashLightPosition[] = { 0, 1, 0, 1 };
	GLfloat flashLightDirection[3];
	if (perspective == 0) {
		flashLightDirection[0] = camera->getForward().getX();
		flashLightDirection[1] = camera->getForward().getY();
		flashLightDirection[2] = camera->getForward().getZ();
	}
	else{
		flashLightDirection[0] = cosf((steve->getAngle() - 90) * PI / 180.0);
		flashLightDirection[1] = -0.4;
		flashLightDirection[2] = sinf((steve->getAngle() - 90) * PI / 180.0);
	}
	GLfloat moonDiffuse[] = { moonD, moonD, moonD, 1 };
	GLfloat moonAmbient[] = { moonA, moonA, moonA, 1 };
	GLfloat moonPosition[] = { 16 + cos(sunAngle) * 100, 100, 16 + sin(sunAngle) * 100, 0 };
	GLfloat sunPosition[] = {  16 + cos(sunAngle) * 100, 100, 16 + sin(sunAngle) * 100, 0 };
	GLfloat sunDiffuse[] = { 1, 1, 1, 1 };
	GLfloat sunAmbient[] = { 1, 1, 1 };

	//Passing the custom models the sun or moon's coordinates for shadow calculations depending on day or night
	if (night) {
		steve->setLightPosition(moonPosition[0], moonPosition[1], moonPosition[2]);
		for (int i = 0; i < numberOfPigs; i++) {
			pigs[i]->setLightPosition(moonPosition[0], moonPosition[1], moonPosition[2]);
		}
		for (int i = 0; i < numberOfCows; i++) {
			cows[i]->setLightPosition(moonPosition[0], moonPosition[1], moonPosition[2]);
		}
	}
	else {
		steve->setLightPosition(sunPosition[0], sunPosition[1], sunPosition[2]);
		for (int i = 0; i < numberOfPigs; i++) {
			pigs[i]->setLightPosition(sunPosition[0], sunPosition[1], sunPosition[2]);
		}
		for (int i = 0; i < numberOfCows; i++) {
			cows[i]->setLightPosition(sunPosition[0], sunPosition[1], sunPosition[2]);
		}
	}

	//Setting up and enabling lights depending on day or night.
	if (night) {
		//Disabling sun.
		glDisable(GL_LIGHT2);
		//Spotlight for flashlight.
		if (flashLight) {
			glPushMatrix(); {
				//Translating to Steve's position.
				glTranslatef(steve->getPosition().getX(), steve->getPosition().getY() + steve->getModel()->getEyesY(), steve->getPosition().getZ());
				//Passing the light functions the lighting data.
				glLightfv(GL_LIGHT0, GL_DIFFUSE, flashLightDiffuse);
				glLightfv(GL_LIGHT0, GL_POSITION, flashLightPosition);
				glLightfv(GL_LIGHT0, GL_AMBIENT, flashLightAmbient);
				glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, flashLightDirection);
				//spread of light.
				glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 25);						
				//Light intensity.
				glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 120);						
				glEnable(GL_LIGHT0);
			}glPopMatrix();
		}
		//If the spotlight is off, disable the light.
		else {
			glDisable(GL_LIGHT0);
		}
		//Moon
		glPushMatrix(); {
			glLightfv(GL_LIGHT1, GL_DIFFUSE, moonDiffuse);
			glLightfv(GL_LIGHT1, GL_POSITION, moonPosition);
			glLightfv(GL_LIGHT1, GL_AMBIENT, moonAmbient);
			glEnable(GL_LIGHT1);
		}glPopMatrix();
	}
	//Day
	else {
		//Disabling night lights.
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		//Sun
		glPushMatrix(); {
			glLightfv(GL_LIGHT2, GL_DIFFUSE, sunDiffuse);
			glLightfv(GL_LIGHT2, GL_POSITION, sunPosition);
			glLightfv(GL_LIGHT2, GL_AMBIENT, sunAmbient);
			glEnable(GL_LIGHT2);
		}glPopMatrix();
	}

	//Skybox
	glDisable(GL_LIGHTING); {
		glPushMatrix(); {
			glTranslatef(camera->getPosition().getX(), camera->getPosition().getY() - 0.25, camera->getPosition().getZ());
			skybox->render();
		}glPopMatrix();
	}glEnable(GL_LIGHTING);

	//Temporary light to test normals of hat.

	/*GLfloat pointLightPosition[] = { s / 2, 17, s / 2 + 1, 1 };
	glLightfv(GL_LIGHT4, GL_DIFFUSE, sunDiffuse);
	glLightfv(GL_LIGHT4, GL_AMBIENT, sunAmbient);
	glLightfv(GL_LIGHT4, GL_POSITION, pointLightPosition);
	glEnable(GL_LIGHT4);
	glPushMatrix(); {
		glTranslatef(s / 2, 17, s / 2 + 1);
		gluSphere(gluNewQuadric(), .2, 20, 20);
	}glPopMatrix();*/


	//Pond reflections.
	//Clearing stencil buffer.
	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST); {											//Do a stencil test.
		glStencilFunc(GL_ALWAYS, 1, 1); {										//Set perameters for stencil test.
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); {							//Set what to do if each pixel passes / fails the stencil test.
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); {					//Sets which color pixels to be set in the frame buffer.
					glDisable(GL_DEPTH_TEST); {												//Doesn't take into account the pixel's depth value so it draws it in the order it is rendered.
																							//These objects we do not want to render. They are like holes in the stencil. 
						glEnableClientState(GL_VERTEX_ARRAY);
						glVertexPointer(3, GL_FLOAT, 0, reflectionStencil.data());
						glDrawArrays(GL_QUADS, 0, reflectionStencil.size() / 3);
						glDisableClientState(GL_VERTEX_ARRAY);

					}glEnable(GL_DEPTH_TEST);
				}glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			}glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}glStencilFunc(GL_EQUAL, 1, 1);
		//Flipping the skybox and translating it to the cameras position.
		glPushMatrix(); {
			glTranslatef(camera->getPosition().getX(), camera->getPosition().getY(), camera->getPosition().getZ());
			glScalef(1.0, -1.0, 1.0);
			skybox->render();
		}glPopMatrix();
		glPushMatrix(); {
			//Rendering reflected objects.
			glPushMatrix(); {
				glTranslatef(s / 2, 10, s / 2 + 1);
				glScalef(1.0, -1.0, 1.0);
				//Boolean is indicating it is morrored.
				rainbow->render(true);
			}glPopMatrix();
			glTranslatef(0, 28, 0);
			glScalef(1.0, -1.0, 1.0);
			steve->render();
			for (int i = 0; i < numberOfPigs; i++) {
				pigs[i]->render();
			}
			for (int i = 0; i < numberOfCows; i++) {
				cows[i]->render();
			}
		}glPopMatrix();
	}glDisable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	//Water blocks.
	for (int y = 0; y < s; y++) {
		for (int z = 0; z < s; z++) {
			for (int x = 0; x < s; x++) {
				if (grid[y][z][x] == 9) {
					calculateVisibleFaces(x, y, z, -1);
					blocks[y][z][x]->render(visibleFaces);
				}
			}
		}
	}
	
	//Other Blocks.
	for (int y = 0; y < s; y++) {
		for (int z = 0; z < s; z++) {
			for (int x = 0; x < s; x++) {
				//If the block exists and is not a "Water" or "Leaf" block.
				if (blocks[y][z][x] != nullptr && grid[y][z][x] != 7 && grid[y][z][x] != 9) {
					calculateVisibleFaces(x, y, z, -1);
					blocks[y][z][x]->render(visibleFaces);
				}
			}
		}
	}

	//Custom models.
	//If not in first person.
	if (perspective != 0) {
		//steve->render();
		steve->renderWithShadow();
	}
	for (int i = 0; i < numberOfPigs; i++) {
		//pigs[i]->render();
		pigs[i]->renderWithShadow();
	}
	for (int i = 0; i < numberOfCows; i++) {
		//cows[i]->render();
		cows[i]->renderWithShadow();
	}
	candle1->render(3);
	candle2->render(4);
	candle3->render(5);

	//Shere indicating where the ray is intersecting with blocks.
	if (perspective == 2) {
		if (displayIndicator) {
			glDisable(GL_TEXTURE_2D); {
				glDisable(GL_LIGHTING); {
					glColor3f(0, 0, 0);
					glPushMatrix(); {
						glTranslatef(pointOfIntersection[0], pointOfIntersection[1], pointOfIntersection[2]);
						gluSphere(gluNewQuadric(), 0.05, 20, 20);
					}glPopMatrix();
				}glEnable(GL_LIGHTING);
			}glEnable(GL_TEXTURE_2D);
			glColor3f(1, 1, 1);
		}
	}

	//Leaf blocks. Rendering transparency last.
	for (int y = 0; y < s; y++) {
		for (int z = 0; z < s; z++) {
			for (int x = 0; x < s; x++) {
				if (grid[y][z][x] == 7) {
					calculateVisibleFaces(x, y, z, -1);
					blocks[y][z][x]->render(visibleFaces);
				}
			}
		}
	}

	//Rainbow. Transparent.
	glPushMatrix(); {
		glTranslatef(s / 2, 14, s / 2);
		rainbow->render(false);
	}glPopMatrix();
	
														//Mirror blocks are broken... :(
	//Mirror Blocks										//I can draw stuff in mirrored world with minimum vertices as no light is affecting it... or maybe i want a mirror world light...?
	//for (int y = 0; y < s; y++) {
	//	for (int z = 0; z < s; z++) {
	//		for (int x = 0; x < s; x++) {
	//			if (grid[y][z][x] == 9) {
	//				for (int i = 0; i < 6; i++) {	
	//					mirror(blocks[y][z][x], i);
	//				}
	//				//mirror(blocks[y][z][x], 5);
	//			}
	//		}
	//	 }
	//}

	//renderTextOutput();

	glutSwapBuffers();
}

//Using the Intersection of Lines and Planes maths I learn in maths lectures to cast a ray from the camera in the direction it is looking and checking to see if it intersects with any of the blocks.
bool Scene::rayCast(BlockSpace* currentBlockSpace) {		//recursive..? Nope!
	// -1 represents that it has not intersected with anything.
	intersectedFace = -1;
	//dislplayIndicator tells the render() function to render the sphere indicator or not. I initialize this to false initially.
	displayIndicator = false;
	//Setting the ray Distance to zero at the begining of each check.
	rayDistance = 0;
	//Setting loop counter to zero.										//loopCounter is just there for a hacky bug fix where it gets stuck in a loop...
	int loopCounter = 0;
	while (rayDistance < rayMaxDistance) {
		//For each face.
		for (int i = 0; i < 6; i++) {
			//If intersectedFace is not -1 then it has allready hit the side of a block space. 
			//If i equals intersectedFace then that is the face that the ray entered the block space and I can skip the face.
			if (i != intersectedFace) {
				//Setting the intersected block space to the block opposite the face the ray has intersected.
				//Passing intersection() the block space and the face the algorithm is currently looking at.
				intersectionBlock = intersection(currentBlockSpace, i);
				//If the ray has intersected this face.
				if (intersectionBlock != nullptr) {
					//Setting intersectedFace to the corresponding face of the intersected block. (If the ray intersects with the right hand face of the block, the block to the right of it's intersected face will be left.)
					switch (i) {
					case 0: intersectedFace = 2; break;
					case 1: intersectedFace = 3; break;
					case 2: intersectedFace = 0; break;
					case 3: intersectedFace = 1; break;
					case 4: intersectedFace = 5; break;
					case 5: intersectedFace = 4; break;
					}
					//Setting the collision block to the block it has collided with.
					collisionBlock = collision(currentBlockSpace, i);
					//If it is a nullptr then there is no block there. Continue with the algorithm.
					//If has been a collision. 
					if (collisionBlock != nullptr) {
						//Set the targeted block to this block.												//I could set this stuff in update if it has returned true as this is not to do with the algorithm...			
						targetedBlock = collisionBlock;
						//Set the targeted face to this face.
						targetedFace = intersectedFace;
						//Indicate that render() can render the intersection sphere.
						displayIndicator = true;
						//Break out of the loop and return true to indicate there has been a collision.		
						return true;
					}
					//If there has not been a collision and it has not reached the edge of the grid.
					else if (((i == 0 && currentBlockSpace->z > 0) ||
						(i == 1 && currentBlockSpace->x < s - 1) ||
						(i == 2 && currentBlockSpace->z < s - 1) ||
						(i == 3 && currentBlockSpace->x > 0) ||
						(i == 4 && currentBlockSpace->y < s - 1) ||
						(i == 5 && currentBlockSpace->y > 0))) {
						//Set the current block space to the intersected block space so that the algorithm can repeat the checks on this block space.
						currentBlockSpace = intersectionBlock;
						//Increment the distance the ray has travelled. 
						rayDistance++;
						//Break out of the loop as there is no need to keep checking for intersections as there will only ever be one.
						break;
					}
					//If it has reached the end of the grid, stop the algorithm and return false, indicating there has been no collision.
					else {
						displayIndicator = false;
						return false;
					}
				}
			}
		}
		if (loopCounter++ > rayDistance) {		//Hacky way of patching a bug where it gets stuck in the loop... meant to say rayMaxDistance but this seems to work as well...
			break;								//There is also a bug if I put blocks at the edge of the world...
		}										//I am running out of time so I will address this later...
	}
	displayIndicator = false;
}

//Checking if ray is intersecting with and of the blocks immediatly in front, to the right, behind, to the left, above or below the block the camera is currently in. 
//Returns the blockspace next to the currentBlockSpace if it intersects with the face adjoining it. Otherwise it returns a nullptr.
BlockSpace* Scene::intersection(BlockSpace* currentBlockSpace, int face) {		
	//The point of intersection will be somewhere on the infinite plane.
	calculatePointOfIntersection(currentBlockSpace, face);
	//Front
	if (face == 0) {
		//If the point of intersection is withing the bounds of the block's face.
		if (pointOfIntersection[0] > currentBlockSpace->x && pointOfIntersection[0] < currentBlockSpace->x + 1 &&	
			pointOfIntersection[1] > currentBlockSpace->y && pointOfIntersection[1] < currentBlockSpace->y + 1 && t >= 0) {
				//returns the blockSpace opposite the front face.
				return &blockSpaces[currentBlockSpace->y][currentBlockSpace->z + 1][currentBlockSpace->x];
		}
		else {
			return nullptr;
		}
	}
	//Right
	else if (face == 1) {
		if (pointOfIntersection[2] > currentBlockSpace->z && pointOfIntersection[2] < currentBlockSpace->z + 1 &&	
			pointOfIntersection[1] > currentBlockSpace->y && pointOfIntersection[1] < currentBlockSpace->y + 1 && t >= 0) {
				return &blockSpaces[currentBlockSpace->y][currentBlockSpace->z][currentBlockSpace->x + 1];
		}
		else {
			return nullptr;
		}
	}
	//Back
	else if (face == 2) {
		if (pointOfIntersection[0] > currentBlockSpace->x && pointOfIntersection[0] < currentBlockSpace->x + 1 && 
			pointOfIntersection[1] > currentBlockSpace->y && pointOfIntersection[1] < currentBlockSpace->y + 1 && t >= 0) {
				return &blockSpaces[currentBlockSpace->y][currentBlockSpace->z - 1][currentBlockSpace->x];
		}
		else {
			return nullptr;
		}
	}
	//Left
	else if (face == 3) {
		if (pointOfIntersection[2] > currentBlockSpace->z && pointOfIntersection[2] < currentBlockSpace->z + 1 && 
			pointOfIntersection[1] > currentBlockSpace->y && pointOfIntersection[1] < currentBlockSpace->y + 1 && t >= 0) {
				return &blockSpaces[currentBlockSpace->y][currentBlockSpace->z][currentBlockSpace->x - 1];
		}
		else {
			return nullptr;
		}
	}
	//Top
	else if (face == 4) {
		if (pointOfIntersection[0] > currentBlockSpace->x && pointOfIntersection[0] < currentBlockSpace->x + 1 &&	
			pointOfIntersection[2] > currentBlockSpace->z && pointOfIntersection[2] < currentBlockSpace->z + 1 && t >= 0) {
				return &blockSpaces[currentBlockSpace->y + 1][currentBlockSpace->z][currentBlockSpace->x];
		}
		else {
			return nullptr;
		}
	}
	//Bottom
	else if (face == 5) {
		if (pointOfIntersection[0] > currentBlockSpace->x && pointOfIntersection[0] < currentBlockSpace->x + 1 && 
			pointOfIntersection[2] > currentBlockSpace->z && pointOfIntersection[2] < currentBlockSpace->z + 1 && t >= 0) {
				return &blockSpaces[currentBlockSpace->y - 1][currentBlockSpace->z][currentBlockSpace->x];
		}
		else {
			return nullptr;
		}
	}
}

//Intersection of lines and planes maths from lectures.
void Scene::calculatePointOfIntersection(BlockSpace* currentBlockSpace, int face)
{
	switch (face) {
	case 0:	//Front
		//Setting the normal of the plane.
		normal[0] = 0;
		normal[1] = 0;
		normal[2] = 1;
		//Calculating the point of the arbitrary point on the plane. (I use 0, 0 for the points that do not describe the plane for simplicity.)
		pointOnPlane[0] = 0;
		pointOnPlane[1] = 0;
		pointOnPlane[2] = currentBlockSpace->z + 1;
		break;
	case 1:	//Right
		normal[0] = 1;
		normal[1] = 0;
		normal[2] = 0;
		pointOnPlane[0] = currentBlockSpace->x + 1;
		pointOnPlane[1] = 0;
		pointOnPlane[2] = 0;
		break;
	case 2:	//Back
		//The normal of the plane here should be (0, 0, -1) but my maths failed and I worked out I could get the same affect with the plane facing the other way.
		normal[0] = 0;
		normal[1] = 0;
		normal[2] = 1;
		pointOnPlane[0] = 0;
		pointOnPlane[1] = 0;
		pointOnPlane[2] = currentBlockSpace->z;
		break;
	case 3:	//Left
		normal[0] = 1;
		normal[1] = 0;
		normal[2] = 0;
		pointOnPlane[0] = currentBlockSpace->x;
		pointOnPlane[1] = 0;
		pointOnPlane[2] = 0;
		break;
	case 4:	//Top
		normal[0] = 0;
		normal[1] = 1;
		normal[2] = 0;
		pointOnPlane[0] = 0;
		pointOnPlane[1] = currentBlockSpace->y + 1;
		pointOnPlane[2] = 0;
		break;
	case 5:	//Bottom
		normal[0] = 0;
		normal[1] = 1;
		normal[2] = 0;
		pointOnPlane[0] = 0;
		pointOnPlane[1] = currentBlockSpace->y;
		pointOnPlane[2] = 0;
		break;
	}
	//Calculating the dot product of the point of the vector of the arbitrary point on the plane and the normal.
	d = 0;
	for (int k = 0; k < 3; k++) {
		d += normal[k] * pointOnPlane[k];
	}
	//Calculating the magnitude of the vector from the camera to the point on the plane.
	t = (normal[0] * (d - camera->getPosition().getX()) / (camera->getForward().getX() + 0.00001)) +
		(normal[1] * (d - camera->getPosition().getY()) / (camera->getForward().getY() + 0.00001)) +
		(normal[2] * (d - camera->getPosition().getZ()) / (camera->getForward().getZ() + 0.00001));
	//Calculating the point of intersection of the line and the plane.
	pointOfIntersection[0] = camera->getPosition().getX() + t * camera->getForward().getX();
	pointOfIntersection[1] = camera->getPosition().getY() + t * camera->getForward().getY();
	pointOfIntersection[2] = camera->getPosition().getZ() + t * camera->getForward().getZ();
	
}

//Returns the block that is next to the currentBlockSpace in the direction of the face.
Block* Scene::collision(BlockSpace* currentBlockSpace, int face)
{	
	//Front
	if (face == 0) {
		return blocks[currentBlockSpace->y][currentBlockSpace->z + 1][currentBlockSpace->x];
	}
	//Right
	else if (face == 1) {
		return blocks[currentBlockSpace->y][currentBlockSpace->z][currentBlockSpace->x + 1];
	}
	//Back
	else if (face == 2) {
		return blocks[currentBlockSpace->y][currentBlockSpace->z - 1][currentBlockSpace->x];
	}
	//Left
	else if (face == 3) {
		return blocks[currentBlockSpace->y][currentBlockSpace->z][currentBlockSpace->x - 1];
	}
	//Top
	else if (face == 4) {
		return blocks[currentBlockSpace->y + 1][currentBlockSpace->z][currentBlockSpace->x];
	}
	//Bottom.
	else if (face == 5) {
		return blocks[currentBlockSpace->y - 1][currentBlockSpace->z][currentBlockSpace->x];
	}
}

//Looping through the top level of the blocks and adding verticies to the shadow stencil.
void Scene::generateShadowStencil()
{
	shadowStencil.clear();
	for (int z = 0; z < s; z++) {
		for (int x = 0; x < s; x++) {
			if (blocks[13][z][x] != nullptr) {
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getX());
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getY() + 1);
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getZ());
				  					  
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getX());
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getY() + 1);
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getZ() + 1);
							  
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getX() + 1);
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getY() + 1);
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getZ() + 1);
				 					  
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getX() + 1);
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getY() + 1);
				shadowStencil.push_back(blocks[13][z][x]->getPosition()->getZ());
			}
		}
	}
	steve->setStencil(&shadowStencil);
}

//Looping through the top level of the blocks and adding verticies to the reflection stencil if it is a water block.
void Scene::generateReflectionStencil()
{
	reflectionStencil.clear();
	for (int z = 0; z < s; z++) {
		for (int x = 0; x < s; x++) {
			if (grid[13][z][x] == 9 && blocks[13][z][x] != nullptr) {
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getX());
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getY() + 1);
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getZ());
				
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getX());
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getY() + 1);
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getZ() + 1);
				
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getX() + 1);
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getY() + 1);
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getZ() + 1);
				
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getX() + 1);
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getY() + 1);
				reflectionStencil.push_back(blocks[13][z][x]->getPosition()->getZ());
			}
		}
	}
}

//Loading the map from a txt file.
void Scene::load()
{
	//Populating my filmap string array with data from the txt file.
	std::ifstream file;
	std::string line;
	file.open("save.txt");
	int layer = -1;
	int row = 0;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			//A string begining with a '#' indicates the start of a new layer in the y direction.
			if (line[0] == '#') {
				layer++;
				row = 0;
				continue;
			}
			fileMap[layer][row++] = line;
		}
		file.close();
	}
	else std::cout << "File not found.";
							
	//Populating my grid integer array with the ascii value of the char - 48 to account for '0' being 48 on the ascii table.
	for (int y = 0; y < s; y++) {
		for (int z = 0; z < s; z++) {
			for (int x = 0; x < s; x++) {
				grid[y][z][x] = fileMap[y][z][x] - 48;
			}
		}
	}
}

//Saving the current maps state to a txt file.
void Scene::save()
{
	std::ofstream file("save.txt");
	std::string line;
	for (int y = 0; y < s; y++) {
		file << "#" << y << std::endl;
		for (int z = 0; z < s; z++) {
			for (int x = 0; x < s; x++) {
				line += std::to_string(grid[y][z][x]);
			}
			file << line << std::endl;
			line = "";
		}
	}
}

void Scene::calculateVisibleFaces(int x, int y, int z, int mirrored = -1)			
{
	// -1 = Not mirrored
	//	0 = Front
	//	1 = Right
	//	2 = Back
	//	3 = Left
	//	4 = Top
	//	5 = Bottom
	// -1 indicates that the block is not mirrored.			//Currently this function is only ever called with -1 as the perameter as the mirrored block is broken.
	if (mirrored == -1) {
		visibleFaces[0] = (((z == s - 1 || (z < s - 1 && (blocks[y][z + 1][x] == nullptr || (grid[y][z + 1][x] == 7)))) && camera->getPosition().getZ() > blocks[y][z][x]->getPosition()->getZ() + 1));	//Front
		visibleFaces[1] = (((x == s - 1 || (x < s - 1 && (blocks[y][z][x + 1] == nullptr || (grid[y][z][x + 1] == 7)))) && camera->getPosition().getX() > blocks[y][z][x]->getPosition()->getX() + 1));	//Right
		visibleFaces[2] = (((z == 0 || (z > 0 && (blocks[y][z - 1][x] == nullptr || (grid[y][z - 1][x] == 7)))) && camera->getPosition().getZ() < blocks[y][z][x]->getPosition()->getZ()));			//Back
		visibleFaces[3] = (((x == 0 || (x > 0 && (blocks[y][z][x - 1] == nullptr || (grid[y][z][x - 1] == 7)))) && camera->getPosition().getX() < blocks[y][z][x]->getPosition()->getX()));			//Left
		visibleFaces[4] = (((y == s - 1 || (y < s - 1 && (blocks[y + 1][z][x] == nullptr || (grid[y + 1][z][x] == 7)))) && camera->getPosition().getY() > blocks[y][z][x]->getPosition()->getY() + 1));	//Top
		visibleFaces[5] = (((y == 0 || (y > 0 && (blocks[y - 1][z][x] == nullptr || (grid[y - 1][z][x] == 7)))) && camera->getPosition().getY() < blocks[y][z][x]->getPosition()->getY()));			//Bottom
	}
	else {
		mirrored == 0 ? 1 : visibleFaces[0] = (z == s - 1 || (z < s - 1 && (blocks[y][z + 1][x] == nullptr || grid[y][z + 1][x] == 7)));	//Front
		mirrored == 1 ? 1 : visibleFaces[1] = (x == s - 1 || (x < s - 1 && (blocks[y][z][x + 1] == nullptr || grid[y][z][x + 1] == 7)));	//Right
		mirrored == 2 ? 1 : visibleFaces[2] = (z == 0 || (z > 0 && (blocks[y][z - 1][x] == nullptr || grid[y][z - 1][x] == 7)));			//Back																																				//Back
		mirrored == 3 ? 1 : visibleFaces[3] = (x == 0 || (x > 0 && (blocks[y][z][x - 1] == nullptr || grid[y][z][x - 1] == 7)));			//Left
		mirrored == 4 ? 1 : visibleFaces[4] = (y == s - 1 || (y < s - 1 && (blocks[y + 1][z][x] == nullptr || grid[y + 1][z][x] == 7))); 	//Top
		mirrored == 5 ? 1 : visibleFaces[5] = (y == 0 || (y > 0 && (blocks[y - 1][z][x] == nullptr || grid[y - 1][z][x] == 7)));			//Bottom
	}
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

}

// Calculates FPS
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	float y = 1.02;
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, fps);
	sprintf_s(text, "Camera x: %f", camera->getPosition().getX());
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "Camera y: %f", camera->getPosition().getY());
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "Camera z: %f", camera->getPosition().getZ());
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "Camera forward x: %f", camera->getForward().getX());
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "Camera forward y: %f", camera->getForward().getY());
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "Camera forward z: %f", camera->getForward().getZ());
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "Camera Yaw: %f", camera->getYaw());
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "Camera Pitch: %f", camera->getPitch());
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "pointOfIntersection[0]: %f", pointOfIntersection[0]);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "pointOfIntersection[1]: %f", pointOfIntersection[1]);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "pointOfIntersection[2]: %f", pointOfIntersection[2]);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "normal[0]: %f", normal[0]);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "normal[1]: %f", normal[1]);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "normal[2]: %f", normal[2]);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "pointOnPlane[0]: %f", pointOnPlane[0]);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "pointOnPlane[1]: %f", pointOnPlane[1]);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "pointOnPlane[2]: %f", pointOnPlane[2]);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "d: %f", d);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "t: %f", t);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "targetedFace: %f", targetedFace);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "displayIndicator x: %i", displayIndicator);
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
	sprintf_s(text, "player angle x: %f", steve->getAngle());
	displayText(-1.f, y -= 0.06, 1.f, 0.f, 0.f, text);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}

//Not used...
//bool Scene::concealed()
//{
//	for (bool face : visibleFaces) {
//		if (face) {
//			return false;
//		}
//	}
//	return true;
//}

//Broken... :(
//void Scene::mirror(Block* block, int face)
//{
//	//	0 = Front				//enums...!		//also this should be a class...
//	//	1 = Right
//	//	2 = Back
//	//	3 = Left
//	//	4 = Top
//	//	5 = Bottom
//
//	Vector3* vertices[4];
//	switch (face) {
//	case 0:	vertices[0] = new Vector3(0, 1, 1);
//		vertices[1] = new Vector3(0, 0, 1);
//		vertices[2] = new Vector3(1, 0, 1);
//		vertices[3] = new Vector3(1, 1, 1);
//		break;
//	case 1: vertices[0] = new Vector3(1, 1, 1);
//		vertices[1] = new Vector3(1, 0, 1);
//		vertices[2] = new Vector3(1, 0, 0);
//		vertices[3] = new Vector3(1, 1, 0);
//		break;
//	case 2: vertices[0] = new Vector3(1, 1, 0);
//		vertices[1] = new Vector3(1, 0, 0);
//		vertices[2] = new Vector3(0, 0, 0);
//		vertices[3] = new Vector3(0, 1, 0);
//		break;
//	case 3: vertices[0] = new Vector3(0, 1, 0);
//		vertices[1] = new Vector3(0, 0, 0);
//		vertices[2] = new Vector3(0, 0, 1);
//		vertices[3] = new Vector3(0, 1, 1);
//		break;
//	case 4: vertices[0] = new Vector3(0, 1, 0);
//		vertices[1] = new Vector3(0, 1, 1);
//		vertices[2] = new Vector3(1, 1, 1);
//		vertices[3] = new Vector3(1, 1, 0);
//		break;
//	case 5: vertices[0] = new Vector3(0, 0, 1);
//		vertices[1] = new Vector3(0, 0, 0);
//		vertices[2] = new Vector3(1, 0, 0);
//		vertices[3] = new Vector3(1, 0, 1);
//		break;
//	}
//
//	glClear(GL_STENCIL_BUFFER_BIT);
//	calculateVisibleFaces(block->getPosition()->getX(), block->getPosition()->getY(), block->getPosition()->getZ(), -1);
//	if (visibleFaces[face]) {		//Incorrect...?
//
//		glEnable(GL_STENCIL_TEST); {											//Do a stencil test.
//			glStencilFunc(GL_ALWAYS, 1, 1); {										//Set perameters for stencil test.
//				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); {							//Set what to do if each pixel passes / fails the stencil test.
//					glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); {					//Sets which color pixels to be set in the frame buffer.
//						glDisable(GL_DEPTH_TEST); {												//Doesn't take into account the pixel's depth value so it draws it in the order it is rendered.
//																								//Drawing stencil.
//							glBegin(GL_QUADS); {
//								for (int i = 0; i < 4; i++) {
//									glVertex3f(block->getPosition()->getX() + vertices[i]->getX(),
//										block->getPosition()->getY() + vertices[i]->getY(),
//										block->getPosition()->getZ() + vertices[i]->getZ());
//								}
//							}glEnd();
//						}glEnable(GL_DEPTH_TEST);
//					}glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//				}glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//			}glStencilFunc(GL_EQUAL, 1, 1);
//
//			//Reflection.
//			glPushMatrix(); {
//				switch (face)
//				{
//				case 0: glTranslatef(0, 0, block->getPosition()->getZ() * 2 + 1);
//					glScalef(1.0, 1.0, -1.0);
//					break;
//				case 1: glTranslatef(block->getPosition()->getX() * 2 + 1, 0, 0);
//					glScalef(-1.0, 1.0, 1.0);
//					break;
//				case 2: glTranslatef(0, 0, block->getPosition()->getZ() * 2 - 1);
//					glScalef(1.0, 1.0, -1.0);
//					break;
//				case 3: glTranslatef(block->getPosition()->getX() * 2 - 1, 0, 0);
//					glScalef(-1.0, 1.0, 1.0);
//					break;
//				case 4: glTranslatef(0, block->getPosition()->getY() * 2 + 1, 0);
//					glScalef(1.0, -1.0, 1.0);
//					break;
//				case 5: glTranslatef(0, block->getPosition()->getY() * 2 - 1, 0);
//					glScalef(1.0, -1.0, 1.0);
//					break;
//				}
//				for (int y = 0; y < s; y++) {												//Stopping stuff being rendered in mirror world if it is behind the mirror when reflected.
//																							//Top																	//Could I use back face culling...? Would it be as efficient...?
//					if (face == 4 && y < s - (s - block->getPosition()->getY()) + 1) {
//						continue;
//					}
//					//Bottom
//					if (face == 5 && y >= block->getPosition()->getY()) {
//						continue;
//					}
//					for (int z = 0; z < s; z++) {
//						//Front																
//						if (face == 0 && z < s - (s - block->getPosition()->getZ())) {
//							continue;
//						}
//						//Back
//						if ((face == 2 && z >= block->getPosition()->getZ())) {
//							break;
//						}
//						for (int x = 0; x < s; x++) {
//							//Right
//							if (face == 1 && x < block->getPosition()->getX() + 1) {
//								continue;
//							}
//							//Left
//							if (face == 3 && x >= block->getPosition()->getX()) {
//								break;
//							}
//							if (blocks[y][z][x] != nullptr && grid[y][z][x] != 9) {
//								glPushMatrix(); {
//									glTranslatef(x, y, z);
//									calculateVisibleFaces(x, y, z, face);
//									blocks[y][z][x]->render(visibleFaces);
//								}glPopMatrix();
//							}
//						}
//					}
//				}
//			}glPopMatrix();
//		}glDisable(GL_STENCIL_TEST);
//		//glClear(GL_STENCIL_BUFFER_BIT);
//		glPushMatrix(); {
//			glTranslatef(block->getPosition()->getX(), block->getPosition()->getY(), block->getPosition()->getZ());
//			calculateVisibleFaces(block->getPosition()->getX(), block->getPosition()->getY(), block->getPosition()->getZ(), -1);
//			block->render(visibleFaces);
//		}glPopMatrix();
//
//		//Drawing mirror block.
//		glEnable(GL_BLEND); {
//			glDisable(GL_LIGHTING); {
//				glBindTexture(GL_TEXTURE_2D, NULL);
//				glColor4f(0.6f, 0.6f, 1.0f, 0.2f);
//				glPushMatrix(); {
//					glBegin(GL_QUADS); {
//						for (int i = 0; i < 4; i++) {
//							glVertex3f(block->getPosition()->getX() + vertices[i]->getX(),
//								block->getPosition()->getY() + vertices[i]->getY(),
//								block->getPosition()->getZ() + vertices[i]->getZ());
//						}
//					}glEnd();
//				}glPopMatrix();
//			}glDisable(GL_LIGHTING);
//		} glDisable(GL_BLEND);
//		glColor4f(1, 1, 1, 1);		//Allways set color back to solid white after blending.
//	}
//	//glClear(GL_STENCIL_BUFFER_BIT);
//}