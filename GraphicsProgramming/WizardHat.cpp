#include "WizardHat.h"

WizardHat::WizardHat(GLuint* texture)
{
	texture_ = texture;
	segments_ = 20;
	height_ = 5;
	segmentHeight_ = 0.6;
	theta_ = (2 * PI) / segments_;
	angle_ = 0;
	level_ = 0;
	radius_ = 0;
	diskRadius = 2;

	calculateCoords();
}


WizardHat::~WizardHat()
{
}

void WizardHat::calculateCoords()
{
	//Cone
	//Vectors for calculating the normals.
	Vector3 a;
	Vector3 b;
	Vector3 c;
	Vector3 ab;
	Vector3 ac;
	Vector3 n;
	angle_ = 0;
	for (int i = height_ - 1; i >= 0; i--) {
		for (int j = 0; j < segments_; j++) {
			//Calculating the normal using the cross product.
			a.setX(cosf(angle_) * (radius_ + 0.2));
			a.setY(i * segmentHeight_);
			a.setZ(sinf(angle_) * (radius_ + 0.2));
			b.setX(cosf(angle_ + theta_) * (radius_ + 0.2));
			b.setY(i * segmentHeight_);
			b.setZ(sinf(angle_ + theta_) * (radius_ + 0.2));
			c.setX(cosf(angle_) * (radius_));
			c.setY(i * segmentHeight_);
			c.setZ(sinf(angle_) * (radius_));
			ab = b - a;
			ac = c - a;
			n = ab.cross(ac);

			conePoints[i][j][0].x = cosf(angle_) * (radius_ + 0.2);
			conePoints[i][j][0].y = i * segmentHeight_;
			conePoints[i][j][0].z = sinf(angle_) * (radius_ + 0.2);
			coneNormals[i][j][0].x = n.x;
			coneNormals[i][j][0].y = n.y;
			coneNormals[i][j][0].z = n.z;
			coneTexturePoints[i][j][1].x = j * (1.0f / segments_);
			coneTexturePoints[i][j][1].y = (height_ - 1 - i) * (1.0f / height_);

			conePoints[i][j][1].x = cosf(angle_) * radius_;
			conePoints[i][j][1].y = (i + 1) * segmentHeight_;
			conePoints[i][j][1].z = sinf(angle_) * radius_;
			coneNormals[i][j][1].x = n.x;
			coneNormals[i][j][1].y = n.y;
			coneNormals[i][j][1].z = n.z;
			coneTexturePoints[i][j][0].x = j * (1.0f / segments_);
			coneTexturePoints[i][j][0].y = ((height_ - 1 - i) + 1) * (1.0f / height_);

			angle_ += theta_;
			conePoints[i][j][2].x = cosf(angle_) * radius_;
			conePoints[i][j][2].y = (i + 1) * segmentHeight_;;
			conePoints[i][j][2].z = sinf(angle_) * radius_;
			coneNormals[i][j][2].x = n.x;
			coneNormals[i][j][2].y = n.y;
			coneNormals[i][j][2].z = n.z;
			coneTexturePoints[i][j][3].x = (j + 1) * (1.0f / segments_);
			coneTexturePoints[i][j][3].y = ((height_ - 1 - i) + 1) * (1.0f / height_);

			conePoints[i][j][3].x = cosf(angle_ ) * (radius_ + 0.2);
			conePoints[i][j][3].y = i * segmentHeight_;
			conePoints[i][j][3].z = sinf(angle_) * (radius_ + 0.2);
			coneNormals[i][j][3].x = n.x;
			coneNormals[i][j][3].y = n.y;
			coneNormals[i][j][3].z = n.z;
			coneTexturePoints[i][j][2].x = (j + 1) * (1.0f / segments_);
			coneTexturePoints[i][j][2].y = (height_ - 1 - i) * (1.0f / height_);
		}
		radius_ += 0.2;
	}

	//Disk.
	angle_ = 0;
	for (int i = 0; i < segments_; i++) {
		//Drawing the disk upright and rotating it in render...
		diskPoints[i][0].x = cosf(angle_) * (diskRadius);
		diskPoints[i][0].y = sinf(angle_) * (diskRadius);
		diskPoints[i][0].z = 0;
		diskNormals[i][0].x = 1;
		diskNormals[i][0].y = 0;
		diskNormals[i][0].z = 0;
		//Indenting the texture coordinates slightly to avoid issue I was having with the seam that GL_REPEAT was not fixing...
		diskTexturePoints[i][0].x = 0.05;
		diskTexturePoints[i][0].y = 0.05;

		diskPoints[i][1].x = 0;
		diskPoints[i][1].y = 0;
		diskPoints[i][0].z = 0;
		diskNormals[i][1].x = 1;
		diskNormals[i][1].y = 0;
		diskNormals[i][1].z = 0;
		diskTexturePoints[i][1].x = 0.05;
		diskTexturePoints[i][1].y = 0.95;

		angle_ += theta_;
		diskPoints[i][2].x = cosf(angle_) * (diskRadius);
		diskPoints[i][2].y = sinf(angle_) * (diskRadius);
		diskPoints[i][2].z = 0;
		diskNormals[i][2].x = 1;
		diskNormals[i][2].y = 0;
		diskNormals[i][2].z = 0;
		diskTexturePoints[i][2].x = 0.95;
		diskTexturePoints[i][2].y = 0.05;
	}
}

void WizardHat::render()
{
	
	glBindTexture(GL_TEXTURE_2D, *texture_);			

	//Cone
	glPushMatrix();
	for (int i = 0; i < segments_; i++) {
		for (int j = 0; j < segments_; j++) {
			glBegin(GL_QUADS);
			for (int k = 0; k < 4; k++) {
				glTexCoord2f(coneTexturePoints[i][j][k].x, coneTexturePoints[i][j][k].y);
				glNormal3f(conePoints[i][j][k].x, conePoints[i][j][k].y, conePoints[i][j][k].z);
				glVertex3f(conePoints[i][j][k].x, conePoints[i][j][k].y, conePoints[i][j][k].z);
			}
			glEnd();
		}
	}
	glPopMatrix();

	//Disc
	glPushMatrix();
	//Rotating the disk to be flat...
	glRotatef(90, 1, 0, 0);
		for (int j = 0; j < segments_; j++) {
			glBegin(GL_TRIANGLES);
			for (int k = 0; k < 4; k++) {
				glTexCoord2f(diskTexturePoints[j][k].x, diskTexturePoints[j][k].y);
				glNormal3f(diskPoints[j][k].x, diskPoints[j][k].y, diskPoints[j][k].z);
				glVertex3f(diskPoints[j][k].x, diskPoints[j][k].y, diskPoints[j][k].z);
			}
			glEnd();
		}
	glPopMatrix();
}
