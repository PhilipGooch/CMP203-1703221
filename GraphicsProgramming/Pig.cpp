#include "Pig.h"


Pig::Pig()
{
	//Loading textures for each cuboid.
	textures[0] = SOIL_load_OGL_texture
	(
		"gfx/pigBody.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	textures[1] = SOIL_load_OGL_texture
	(
		"gfx/pigHead.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	textures[2] = SOIL_load_OGL_texture
	(
		"gfx/pigLeg.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	textures[3] = textures[2];
	textures[4] = textures[2];
	textures[5] = textures[2];

	textures[6] = SOIL_load_OGL_texture
	(
		"gfx/pigNose.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	//Creating a new Box object for each of the cuboids making up the pig. I pass the relevant texture, size (in minecraft pixels) and scale of the object.
	body = new Box(&textures[0], 10, 8, 16, size);
	head = new Box(&textures[1], 8, 8, 8, size);
	leftArm = new Box(&textures[2], 4, 6, 4, size);
	rightArm = new Box(&textures[3], 4, 6, 4, size);
	leftLeg = new Box(&textures[4], 4, 6, 4, size);
	rightLeg = new Box(&textures[5], 4, 6, 4, size);
	nose = new Box(&textures[6], 4, 3, 1, size);

	//Setting the starting direction for the limb animations.
	limbDirections[0] = 1;
	limbDirections[1] = -1;
	limbDirections[2] = 1;
	limbDirections[3] = -1;
	//Setting the angles of each of the limbs to zero for when he is not moving.
	limbAngles[0] = 0;
	limbAngles[1] = 0;
	limbAngles[2] = 0;
	limbAngles[3] = 0;
	//How fast the limbs move int the animation.
	limbSpeed = 30;

	//Setting the positions of each of the cuboids relative to each other.
	bodyOffset.set(0, body->getHeight() / 2 + leftLeg->getHeight(), 0);
	headOffset.set(0, body->getHeight() / 2 - head->getHeight() / 4, -body->getDepth() / 2);
	leftArmOffset.set(-body->getWidth() / 2 + leftArm->getWidth() / 2, -body->getHeight() / 2 - leftArm->getHeight() / 2, -body->getDepth() / 2 + leftArm->getDepth() / 2);
	rightArmOffset.set(body->getWidth() / 2 - rightArm->getWidth() / 2, -body->getHeight() / 2 - rightArm->getHeight() / 2, -body->getDepth() / 2 + rightArm->getDepth() / 2);
	leftLegOffset.set(-body->getWidth() / 2 + leftLeg->getWidth() / 2, -body->getHeight() / 2 - leftLeg->getHeight() / 2, body->getDepth() / 2 - leftLeg->getDepth() / 2);
	rightLegOffset.set(body->getWidth() / 2 - rightLeg->getWidth() / 2, -body->getHeight() / 2 - rightLeg->getHeight() / 2, body->getDepth() / 2 - leftLeg->getDepth() / 2);
	noseOffset.set(0, -nose->getHeight() / 2, -head->getDepth() / 2 - nose->getDepth() / 2);

	//Setting the vector for translating the limbs to the joints for translating, rotating and translating back to get required animation.
	limbAnimationTranslate.set(0, leftArm->getHeight() / 2, 0);
}

Pig::~Pig()
{

}

void Pig::render()
{
	//Hierarchical modeling to allow for animations.
	glPushMatrix(); {
		glTranslatef(position.getX(), position.getY(), position.getZ());
		glTranslatef(bodyOffset.getX(), bodyOffset.getY(), bodyOffset.getZ());
		glRotatef(-angle - 90, 0, 1, 0);
		//Body
		body->render();
		glPushMatrix(); {
			//Head
			glTranslatef(headOffset.getX(), headOffset.getY(), headOffset.getZ());
			head->render();
			//Nose
			glPushMatrix(); {
				glTranslatef(noseOffset.getX(), noseOffset.getY(), noseOffset.getZ());
				nose->render();
			}glPopMatrix();
		}glPopMatrix();
		//Left Arm
		glPushMatrix(); {
			//Translating, rotating and translating back to achieve animation.
			glTranslatef(leftArmOffset.getX(), leftArmOffset.getY(), leftArmOffset.getZ());
			glTranslatef(limbAnimationTranslate.getX(), limbAnimationTranslate.getY(), limbAnimationTranslate.getZ());
			glRotatef(limbAngles[0], 1, 0, 0);
			glTranslatef(-limbAnimationTranslate.getX(), -limbAnimationTranslate.getY(), -limbAnimationTranslate.getZ());
			leftArm->render();
		}glPopMatrix();
		//Right Arm
		glPushMatrix(); {
			glTranslatef(rightArmOffset.getX(), rightArmOffset.getY(), rightArmOffset.getZ());
			glTranslatef(limbAnimationTranslate.getX(), limbAnimationTranslate.getY(), limbAnimationTranslate.getZ());
			glRotatef(limbAngles[1], 1, 0, 0);
			glTranslatef(-limbAnimationTranslate.getX(), -limbAnimationTranslate.getY(), -limbAnimationTranslate.getZ());
			rightArm->render();
		}glPopMatrix();
		//Left Leg
		glPushMatrix(); {
			glTranslatef(leftLegOffset.getX(), leftLegOffset.getY(), leftLegOffset.getZ());
			glTranslatef(limbAnimationTranslate.getX(), limbAnimationTranslate.getY(), limbAnimationTranslate.getZ());
			glRotatef(limbAngles[2], 1, 0, 0);
			glTranslatef(-limbAnimationTranslate.getX(), -limbAnimationTranslate.getY(), -limbAnimationTranslate.getZ());
			leftLeg->render();
		}glPopMatrix();
		//Right Leg
		glPushMatrix(); {
			glTranslatef(rightLegOffset.getX(), rightLegOffset.getY(), rightLegOffset.getZ());
			glTranslatef(limbAnimationTranslate.getX(), limbAnimationTranslate.getY(), limbAnimationTranslate.getZ());
			glRotatef(limbAngles[3], 1, 0, 0);
			glTranslatef(-limbAnimationTranslate.getX(), -limbAnimationTranslate.getY(), -limbAnimationTranslate.getZ());
			rightLeg->render();
		}glPopMatrix();
	}glPopMatrix();
}

void Pig::renderWithShadow()
{
	
	//Passing the shadow matrix the data it needs to squish steve into a shadow depending on the lights position.
	shadow.generateShadowMatrix(shadowMatrix, lightPosition, floorVerts);

	//The pigs never go to the edge so I have not included the stencil for them...

	//glEnable(GL_STENCIL_TEST); {
	//	glStencilFunc(GL_ALWAYS, 1, 1); {
	//		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); {
	//			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); {
	//				glDisable(GL_DEPTH_TEST); {
	//					//Drawing stencil.
	//					glBegin(GL_QUADS); {
	//						glVertex3f(16, 14, 16);
	//						glVertex3f(16, 14, 21);
	//						glVertex3f(21, 14, 21);
	//						glVertex3f(21, 14, 16);
	//					}glEnd();
	//				}glEnable(GL_DEPTH_TEST);
	//			}glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//		}glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//	}glStencilFunc(GL_EQUAL, 1, 1);

	//Shadow Pig
	glDisable(GL_TEXTURE_2D); {
		glDisable(GL_LIGHTING); {
			//Different color shadows for day and night.
			if (night) {
				glColor3f(0, 0, 0);
			}
			else {
				glColor3f(0.1f, 0.1f, 0.1f);
			}
			glPushMatrix(); {
				//Apply the shadow matrix.
				glMultMatrixf((GLfloat *)shadowMatrix);
				//Render the shadow.
				render();
			}glPopMatrix();
			glColor3f(1.0f, 1.0f, 1.0f); 
		}glEnable(GL_LIGHTING);
	}glEnable(GL_TEXTURE_2D);
	//}glDisable(GL_STENCIL_TEST);

	//Real Pig
	render();
}
