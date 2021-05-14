#include "Steve.h"

Steve::Steve(Model models_[3])
{
	models = models_;

	//Loading textures for each cuboid.
	textures[0] = SOIL_load_OGL_texture
	(
		"gfx/playerBody.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	textures[1] = SOIL_load_OGL_texture
	(
		"gfx/playerHead.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	textures[2] = SOIL_load_OGL_texture
	(
		"gfx/playerLeftArm.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	textures[3] = SOIL_load_OGL_texture
	(
		"gfx/playerRightArm.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	textures[4] = SOIL_load_OGL_texture
	(
		"gfx/playerLeftLeg.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	textures[5] = SOIL_load_OGL_texture
	(
		"gfx/playerRightLeg.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	//Creating a new Box object for each of the cuboids making up Steve. I pass the relevant texture, size (in minecraft pixels) and scale of the object.
	body = new Box(&textures[0], 8, 12, 4, size);
	head = new Box(&textures[1], 8, 8, 8, size);
	leftArm = new Box(&textures[2], 4, 12, 4, size);
	rightArm = new Box(&textures[3], 4, 12, 4, size);
	leftLeg = new Box(&textures[4], 4, 12, 4, size);
	rightLeg = new Box(&textures[5], 4, 12, 4, size);

	//Setting the starting direction for the limb animations.
	limbDirections[0] = 1;
	limbDirections[1] = -1;
	limbDirections[2] = -1;
	limbDirections[3] = 1;
	//Setting the angles of each of the limbs to zero for when he is not moving.
	limbAngles[0] = 0;
	limbAngles[1] = 0;
	limbAngles[2] = 0;
	limbAngles[3] = 0;
	//How fast the limbs move int the animation.
	limbSpeed = 30;

	//Setting the positions of each of the cuboids relative to each other.
	bodyOffset.set(0, body->getHeight() / 2 + leftLeg->getHeight(), 0);
	headOffset.set(0, body->getHeight() / 2 + head->getHeight() / 2, 0);
	leftArmOffset.set(-body->getWidth() / 2 - leftArm->getWidth() / 2, body->getHeight() / 2 - leftArm->getHeight() / 2, 0);
	rightArmOffset.set(body->getWidth() / 2 + rightArm->getWidth() / 2, body->getHeight() / 2 - leftArm->getHeight() / 2, 0);
	leftLegOffset.set(-body->getWidth() / 2 + leftLeg->getWidth() / 2, -body->getHeight() / 2 - leftLeg->getHeight() / 2, 0);
	rightLegOffset.set(body->getWidth() / 2 - rightLeg->getWidth() / 2, -body->getHeight() / 2 - rightLeg->getHeight() / 2, 0);

	//Float to hold the y position of his eyes. This is used for the first person camera.
	eyesY = leftLeg->getHeight() + body->getHeight() + head->getHeight() / 2;

	//Setting the vector for translating the limbs to the joints for translating, rotating and translating back to get required animation.
	limbAnimationTranslate.set(0, leftArm->getHeight() / 2, 0);

	//Loading in hat texture.
	wizardHatTexture = SOIL_load_OGL_texture
	(
		"gfx/obsidian.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	//Creating instance of my wizardHat object and passing it is texture.
	wizardHat = new WizardHat(&wizardHatTexture);
}

Steve::~Steve()
{

}

void Steve::render()
{
	//Hierarchical modeling to allow for animations.
	glPushMatrix(); {
		glTranslatef(position.getX(), position.getY(), position.getZ());
		glTranslatef(bodyOffset.getX(), bodyOffset.getY(), bodyOffset.getZ());
		glRotatef(-angle - strafeDirection, 0, 1, 0);
		//Body
		body->render();
		//Head
		glPushMatrix(); {
			glTranslatef(headOffset.getX(), headOffset.getY(), headOffset.getZ());
			head->render();
			//Hat
			if (hat) {
				glPushMatrix(); {
					glTranslatef(0, 0.26, 0);
					glScalef(0.25, 0.25, 0.25);
					wizardHat->render();
				}glPopMatrix();
			}
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
			glTranslatef(-limbAnimationTranslate.getX(), -limbAnimationTranslate.getY(), -limbAnimationTranslate.getZ());
			//Translating and rotating the imported models into their poisitions in his right hand. I render these inside the matrix stack level of the right arm so the transformations will apply to these as well in the animation.
			if (toolIndex == 0) {
				glTranslatef(0, 0.05, -0.25);
				glRotatef(-45, 1, 0, 0);
				glRotatef(90, 0, 0, 1);
			}
			else if (toolIndex == 1) {
				glTranslatef(0, 0.095, -0.25);
				glRotatef(-45, 1, 0, 0);
				glRotatef(90, 0, 0, 1);
			}
			else if (toolIndex == 2) {
				glTranslatef(0, 0.1, -0.40);
				glRotatef(90, 0, 0, 1);
				glRotatef(-45, 0, 1, 0);
			}
			glScalef(0.03125, 0.03125, 0.03125);
			models[toolIndex].render();
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

void Steve::renderWithShadow()
{	
	//Passing the shadow matrix the data it needs to squish steve into a shadow depending on the lights position.
	shadow.generateShadowMatrix(shadowMatrix, lightPosition, floorVerts);

	//Doing a stencil test to only allow the shadow to be seen at specified points.
	glEnable(GL_STENCIL_TEST); {
		glStencilFunc(GL_ALWAYS, 1, 1); {
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); {
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); {
					glDisable(GL_DEPTH_TEST); {
						//Drawing stencil. This stencil shape is generated in scene from the top layer of blocks and is passed down to this class.
						glEnableClientState(GL_VERTEX_ARRAY);
						glVertexPointer(3, GL_FLOAT, 0, stencil->data());
						glDrawArrays(GL_QUADS, 0, stencil->size() / 3);
						glDisableClientState(GL_VERTEX_ARRAY);
					}glEnable(GL_DEPTH_TEST);
				}glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			}glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}glStencilFunc(GL_EQUAL, 1, 1);
		
		//Shadow Steve
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
	}glDisable(GL_STENCIL_TEST);
	//glClear(GL_STENCIL_BUFFER_BIT);
	
	//Real Steve
	render();
}





