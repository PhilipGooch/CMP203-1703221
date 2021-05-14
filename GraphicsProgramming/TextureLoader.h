#pragma once
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "SOIL.h"

//A simple class that handles loading all my textures.
class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	GLuint grassSide;
	GLuint grassTop;
	GLuint earth;
	GLuint mirror;
	GLuint sand;
	GLuint gravel;
	GLuint stone;
	GLuint woodTop;
	GLuint woodSide;
	GLuint redStoneShell;
	GLuint redStoneSparkle;

	GLuint water;
	GLuint leaves;

	GLuint red;
	GLuint green;
	GLuint blue;
	GLuint yellow;
	GLuint cyan;
	GLuint pink;
	GLuint black;
};

