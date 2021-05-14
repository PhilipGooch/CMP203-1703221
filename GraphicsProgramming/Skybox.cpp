#include "Skybox.h"



Skybox::Skybox(GLuint* dayTexture, GLuint* nightTexture)
{
	dayTexture_ = dayTexture;
	nightTexture_ = nightTexture;
	currentTexture_ = nightTexture_;
}


Skybox::~Skybox()
{
}

void Skybox::render()
{
	//Disabling depth test so that it will render first and not care about it's depth value.
	glDisable(GL_DEPTH_TEST);
	//Disabling lighting as otherwize it would be obvious it was just a box.
	glDisable(GL_LIGHTING);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glBindTexture(GL_TEXTURE_2D, *currentTexture_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_TRIANGLES);

	//front								//Early code... No point in using triangles as it is flat... No point in setting normals as lighting is disabled... 
	glTexCoord2f(0.25f, 0.25f);
	glNormal3f(0, 0, 1);
	glVertex3f(-1, 1, 1);

	glTexCoord2f(0.25f, 0.5f);
	glNormal3f(0, 0, 1);
	glVertex3f(-1, -1, 1);

	glTexCoord2f(0.5f, 0.5f);
	glNormal3f(0, 0, 1);
	glVertex3f(1, -1, 1);

	glTexCoord2f(0.25f, 0.25f);
	glNormal3f(0, 0, 1);
	glVertex3f(-1, 1, 1);

	glTexCoord2f(0.5f, 0.5f);
	glNormal3f(0, 0, 1);
	glVertex3f(1, -1, 1);

	glTexCoord2f(0.5f, 0.25f);
	glNormal3f(0, 0, 1);
	glVertex3f(1, 1, 1);

	//right 
	glTexCoord2f(0.5f, 0.25f);
	glNormal3f(1, 0, 0);
	glVertex3f(1, 1, 1);

	glTexCoord2f(0.5f, 0.5f);
	glNormal3f(1, 0, 0);
	glVertex3f(1, -1, 1);

	glTexCoord2f(0.75f, 0.5f);
	glNormal3f(1, 0, 0);
	glVertex3f(1, -1, -1);

	glTexCoord2f(0.5f, 0.25f);
	glNormal3f(1, 0, 0);
	glVertex3f(1, 1, 1);

	glTexCoord2f(0.75f, 0.5f);
	glNormal3f(1, 0, 0);
	glVertex3f(1, -1, -1);

	glTexCoord2f(0.75f, 0.25f);
	glNormal3f(1, 0, 0);
	glVertex3f(1, 1, -1);

	//back 
	glTexCoord2f(0.75f, 0.25f);
	glNormal3f(0, 0, -1);
	glVertex3f(1, 1, -1);

	glTexCoord2f(0.75f, 0.5f);
	glNormal3f(0, 0, -1);
	glVertex3f(1, -1, -1);

	glTexCoord2f(1.f, 0.5f);
	glNormal3f(0, 0, -1);
	glVertex3f(-1, -1, -1);

	glTexCoord2f(0.75f, 0.25f);
	glNormal3f(0, 0, -1);
	glVertex3f(1, 1, -1);

	glTexCoord2f(1.f, 0.5f);
	glNormal3f(0, 0, -1);
	glVertex3f(-1, -1, -1);

	glTexCoord2f(1.f, 0.25f);
	glNormal3f(0, 0, -1);
	glVertex3f(-1, 1, -1);

	//left 
	glTexCoord2f(0.f, 0.25f);
	glNormal3f(-1, 0, 0);
	glVertex3f(-1, 1, -1);

	glTexCoord2f(0.f, 0.5f);
	glNormal3f(-1, 0, 0);
	glVertex3f(-1, -1, -1);

	glTexCoord2f(0.25f, 0.5f);
	glNormal3f(-1, 0, 0);
	glVertex3f(-1, -1, 1);

	glTexCoord2f(0.f, 0.25f);
	glNormal3f(-1, 0, 0);
	glVertex3f(-1, 1, -1);

	glTexCoord2f(0.25f, 0.5f);
	glNormal3f(-1, 0, 0);
	glVertex3f(-1, -1, 1);

	glTexCoord2f(0.25f, 0.25f);
	glNormal3f(-1, 0, 0);
	glVertex3f(-1, 1, 1);

	//top 
	glTexCoord2f(0.25f, 0.f);
	glNormal3f(0, 1, 0);
	glVertex3f(-1, 1, -1);

	glTexCoord2f(0.25f, 0.25f);
	glNormal3f(0, 1, 0);
	glVertex3f(-1, 1, 1);

	glTexCoord2f(0.5f, 0.25f);
	glNormal3f(0, 1, 0);
	glVertex3f(1, 1, 1);

	glTexCoord2f(0.25f, 0.f);
	glNormal3f(0, 1, 0);
	glVertex3f(-1, 1, -1);

	glTexCoord2f(0.5f, 0.25f);
	glNormal3f(0, 1, 0);
	glVertex3f(1, 1, 1);

	glTexCoord2f(0.5f, 0.f);
	glNormal3f(0, 1, 0);
	glVertex3f(1, 1, -1);

	//bottom 
	glTexCoord2f(0.25f, 0.75f);
	glNormal3f(0, -1, 0);
	glVertex3f(-1, -1, -1);

	glTexCoord2f(0.25f, 0.5f);
	glNormal3f(0, -1, 0);
	glVertex3f(-1, -1, 1);

	glTexCoord2f(0.5f, 0.5f);
	glNormal3f(0, -1, 0);
	glVertex3f(1, -1, 1);

	glTexCoord2f(0.25f, 0.75f);
	glNormal3f(0, -1, 0);
	glVertex3f(-1, -1, -1);

	glTexCoord2f(0.5f, 0.5f);
	glNormal3f(0, -1, 0);
	glVertex3f(1, -1, 1);

	glTexCoord2f(0.5f, 0.75f);
	glNormal3f(0, -1, 0);
	glVertex3f(1, -1, -1);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void Skybox::switchTexture()
{
	currentTexture_ == dayTexture_ ? currentTexture_ = nightTexture_ : currentTexture_ = dayTexture_;
}

void Skybox::switchWireFrame()
{
	wireframe = !wireframe;
}
