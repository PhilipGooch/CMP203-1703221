#pragma once
#include "MyModel.h"
#include "Vector3.h"
#include "glut.h"
#include "SOIL.h"
#include "Box.h"
#include "Model.h"
#include "WizardHat.h"

//Steve inherits from MyModel
class Steve : public MyModel
{
public:
	//Being passed the imported models of the sword, axe and pickaxe.
	Steve(Model models[3]);
	~Steve();
	void render();
	virtual void renderWithShadow();

protected:
	//Imorted models.
	Model* models;

	//Wizard Hat.
	GLuint wizardHatTexture;
	WizardHat* wizardHat;
	
};

