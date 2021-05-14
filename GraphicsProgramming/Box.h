#pragma once
#include "glut.h"
#include <utility>
#include <vector>
#include <string>

class Box			//Rename to cuboid...
{
public:
	Box(GLuint* bodyTopTexture_, float width_, float height_, float depth_, float size);
	~Box();

	//Calculates the texture coordinates for any sized cuboid given an image in the format I specify.
	void calculateCoords();

	void render();			//I calculate the vertices and normals inside render()...	I wrote this code before learning about vertex arrays...

	//Getters
	float getWidth();
	float getHeight();
	float getDepth();

private:
	//For texture coordinates.
	struct TexturePoint {		
		float x, y;
	};

	float width, height, depth;

	//Pointer to it's texture
	GLuint* texture;

	//This stores the positions of each of the texture points I calculate.
	TexturePoint texturePoints[14];

};

