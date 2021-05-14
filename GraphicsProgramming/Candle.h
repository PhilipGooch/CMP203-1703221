#pragma once
#include "MyModel.h"
#include "Vector3.h"
#include "glut.h"
#include "Box.h"
#include "SOIL.h"

//Candle inherits from MyModel.
class Candle : public MyModel
{
public:
	Candle();
	~Candle();
	//Passing an integer specifying which light the candle is using.
	virtual void render(int light);
	virtual void renderWithShadow();
};

