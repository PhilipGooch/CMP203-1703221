#include "Rainbow.h"



Rainbow::Rainbow(GLuint* texture, float radius, float width)
{
	texture_ = texture;
	segments_ = 100;
	radius_ = radius;
	width_ = width;
	theta_ = (2 * PI) / segments_;
	angle_ = 0;

	calculateCoords();
}


Rainbow::~Rainbow()
{
}

void Rainbow::calculateCoords()
{
	//Populating vertex array with rainbow coordinates.
	angle_ = 0;
	for (int i = 0; i < segments_; i++) {
		points[i][0].x = cosf(angle_) * (radius_ + width_);
		points[i][0].y = sinf(angle_) * (radius_ + width_);
		points[i][0].z = 0;
		texturePoints[i][0].x = 0.05;
		texturePoints[i][0].y = 0.05;

		points[i][1].x = cosf(angle_) * radius_;
		points[i][1].y = sinf(angle_) * radius_;
		points[i][0].z = 0;
		texturePoints[i][1].x = 0.05;
		texturePoints[i][1].y = 0.95;

		angle_ += theta_;
		points[i][2].x = cosf(angle_) * radius_;
		points[i][2].y = sinf(angle_) * radius_;
		points[i][2].z = 0;
		texturePoints[i][2].x = 0.95;
		texturePoints[i][2].y = 0.95;

		points[i][3].x = cosf(angle_) * (radius_ + width_);
		points[i][3].y = sinf(angle_) * (radius_ + width_);
		points[i][3].z = 0;
		texturePoints[i][3].x = 0.95;
		texturePoints[i][3].y = 0.05;
	}
}

//Passing it if it is mirrored or not.
void Rainbow::render(bool mirrored)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//Disabling lighting as lighting on a rainbow does not make sense.	(Procedurally generated shapes with lighting and normal calculations can be found in the wizardHat class with the cone.)
	glDisable(GL_LIGHTING);
	//Enabling blending for transparency.
	glEnable(GL_BLEND);
	
	glBindTexture(GL_TEXTURE_2D, *texture_);				

	glPushMatrix();
	if (!mirrored) {
		for (int i = 0; i < segments_; i++) {
			glBegin(GL_QUADS);
			for (int j = 0; j < 4; j++) {
				//No normals are used as lighting is not being used on it.
				glTexCoord2f(texturePoints[i][j].x, texturePoints[i][j].y);
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
			}
			glEnd();
		}
	}
	//If it is mirrored I am only drawing the half of it I can see.
	else {
		for (int i = 0; i < segments_ / 2; i++) {
			glBegin(GL_QUADS);
			for (int j = 0; j < 4; j++) {
				glTexCoord2f(texturePoints[i][j].x, texturePoints[i][j].y);
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
			}
			glEnd();
		}
	}
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
}
