#pragma once
#include "glut.h"
#include <vector>
#include "Vector3.h"

class Block
{
public:
	//Block expects a pointer to a Texture pointer. I am passing it a pointer to an array of texture pointers which is essentially passing a pointer to the first pointer in the array.
	Block(GLuint** textures, int type, Vector3 position);
	Block(GLuint** textures, GLuint** textures2, int type, Vector3 position);
	~Block();

	//Turns wire frame on and off.
	void switchWireFrame();

	//Calculating and populating the vertex, normal and texture coordinate arrays.
	void calculateVertexArrays();

	//Returns a pointer to it's grid position.			//No need for this to be a pointer...
	Vector3* getPosition();

	//Passing the function a pointer to the first element of an array of booleans indicating which faces are currently visible from the camera.
	void render(bool* visibleFaces);

	//Not used...
	//bool getEnclosed();
	//void setEnclosed(bool);
	
private:
	//Pointer to a texture pointer.
	GLuint** textures_;

	//This is used for "ore" blocks that have two textures.
	GLuint** textures2_;
	
	//Visible faces
	bool faces[6];					

	//The type of block.		//enums...!
	int type_;				

	//It's grid position.
	Vector3 position_;

	//Wireframe mode.
	bool wireFrame;

	//Divisions. The number of times the block is divided.
	static const int div = 16;		

	//I counted the number of vertices, normals, texture coordinates and indices I would need for a square divided by a variable to get a sequence of numbers.
	//I then used www.wolframalpha.com to generate an equation I could use to determine the size of arrays I would need depending on the number of divisions.
	// 3 * d^2 + 2d + 1
	float vertices[6][3 * (div * div + 2 * div + 1)];
	float normals[6][3 * (div * div + 2 * div + 1)];
	// 2 * d^2 + 2d + 1
	float textureCoords[6][2 * (div * div + 2 * div + 1)];
	// 4 * d^2
	GLushort indices[6][4 * div * div];


	//Not used...
	//bool enclosed_;
};