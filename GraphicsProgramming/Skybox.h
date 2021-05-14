#pragma once
#include <glut.h>

class Skybox
{
public:
	Skybox(GLuint* dayTexture, GLuint* nightTexture);
	~Skybox();

	void render();

	//Switches from day to night textures.
	void switchTexture();

	//Turns wire frame on and off.
	void switchWireFrame();

private:
	GLuint* dayTexture_;
	GLuint* nightTexture_;
	GLuint* currentTexture_;

	bool wireframe = false;
};

