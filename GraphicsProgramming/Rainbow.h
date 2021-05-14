#pragma once
#include "glut.h"
#include <utility>
#include <vector>
#include <string>

#define PI 3.14159265359

class Rainbow
{
public:
	Rainbow(GLuint* texture, float radius, float width);
	~Rainbow();

	void calculateCoords();

	void render(bool mirrored);



private:
	GLuint* texture_;

	struct TexturePoint {		//For texture coordinates.
		float x, y;
	};

	struct Point {			//For Verticies.
		float x, y, z;
	};

	Point points[100][4];

	TexturePoint texturePoints[100][4];

	//Amount of quads in the rainbow. Each quad is the ful width of the rainbow.
	//Setting it to a float as i need to use this to calcualte texture coordinates.
	float segments_;			

	//The radius of the circle the rainbow is drawn around.
	float radius_;

	//Width of the rainbow.
	float width_;

	//The amount I increment the angle by.
	float theta_;

	float angle_;

};

