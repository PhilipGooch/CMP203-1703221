#pragma once
#include "MyModel.h"
#include "Vector3.h"
#include "glut.h"
#include "Box.h"
#include "SOIL.h"

//Cow inherits from MyModel.
class Cow : public MyModel
{
public:
	Cow();
	~Cow();
	virtual void render();
	virtual void renderWithShadow();
};

