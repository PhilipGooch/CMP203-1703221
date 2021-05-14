#pragma once
#include "MyModel.h"
#include "Vector3.h"
#include "glut.h"
#include "Box.h"
#include "SOIL.h"

//Pig inherits from MyModel
class Pig : public MyModel
{

public:
	Pig();
	~Pig();
	virtual void render();
	virtual void renderWithShadow();
};

