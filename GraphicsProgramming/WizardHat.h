#pragma once
#include "glut.h"
#include <utility>
#include <vector>
#include <string>
#include "Vector3.h"

#define PI 3.14159265359

class WizardHat
{
public:
	WizardHat(GLuint* texture);
	~WizardHat();

	//Populating vertex arrays.
	void calculateCoords();

	void render();

private:
	GLuint * texture_;

	//For texture coordinates.
	struct TexturePoint {		
		float x, y;
	};

	//For Verticies.
	struct Point {				
		float x, y, z;
	};

	//Cone
	Point conePoints[20][20][4];
	Point coneNormals[20][20][4];
	TexturePoint coneTexturePoints[20][20][4];
	float segments_;			
	float height_;
	float segmentHeight_;
	float theta_;
	float angle_;
	float level_;
	float radius_;

	//Disk
	Point diskPoints[20][3];
	Point diskNormals[20][3];
	TexturePoint diskTexturePoints[20][3];		
	float diskRadius;
};

