#include "Candle.h"

Candle::Candle()							
{
	//Loading textures for each cuboid.
	textures[0] = SOIL_load_OGL_texture
	(
		"gfx/torchHandle.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	textures[1] = SOIL_load_OGL_texture
	(
		"gfx/torchFlame.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	//Creating a new Box object for each of the cuboids making up the candle. I pass the relevant texture, size (in minecraft pixels) and scale of the object.
	body = new Box(&textures[0], 2, 8, 2, size);
	head = new Box(&textures[1], 2, 2, 2, size);

	//Setting the positions of each of the cuboids relative to each other.
	bodyOffset.set(0, body->getHeight() / 2, 0);
	headOffset.set(0, body->getHeight() - head->getHeight() - 0.062, 0);


}

Candle::~Candle()
{

}

void Candle::render(int light)
{
	//Setting up variables for point light. Making it red.
	GLfloat diffuse[] = { 1, 0, 0, 1};
	GLfloat ambient[] = { 0, 0, 0, 1 };
	GLfloat lightPosition[] = { 0, 0, 0 };
	glPushMatrix(); {
		glTranslatef(position.getX(), position.getY(), position.getZ());
		glTranslatef(bodyOffset.getX(), bodyOffset.getY(), bodyOffset.getZ());
		//Body	(Candle stick)
		body->render();
		glPushMatrix(); {
			//Head	(Flame)
			glTranslatef(headOffset.getX(), headOffset.getY(), headOffset.getZ());
			glPushMatrix(); {
				//Creating a point light.
				//Setting the lights attenuation (the distance the light travels) so that it falls off quickly. 
				float brightness = 1;
				if (light == 3) {
					glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);
					glLightfv(GL_LIGHT3, GL_POSITION, ambient);
					glLightfv(GL_LIGHT3, GL_AMBIENT, lightPosition);
					glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0 * brightness);
					glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.25 * brightness);
					glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.15 * brightness);					glEnable(GL_LIGHT3);
				}
				else if (light == 4) {
					glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuse);
					glLightfv(GL_LIGHT4, GL_POSITION, ambient);
					glLightfv(GL_LIGHT4, GL_AMBIENT, lightPosition);
					glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1.0 * brightness);
					glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.25 * brightness);
					glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.15 * brightness);					glEnable(GL_LIGHT4);
				}
				else if (light == 5) {
					glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuse);
					glLightfv(GL_LIGHT5, GL_POSITION, ambient);
					glLightfv(GL_LIGHT5, GL_AMBIENT, lightPosition);
					glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 1.0 * brightness);
					glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.25 * brightness);
					glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.15 * brightness);					glEnable(GL_LIGHT5);
				}
			}glPopMatrix();
			//Disabling lighting on the flame of the candle as this is supposed to be the light source.
			glDisable(GL_LIGHTING); {
				head->render();
			}glEnable(GL_LIGHTING);
		}glPopMatrix();
	}glPopMatrix();
}

//This function is not actually called as I do not want to render there to be a shadow from something that is emitting light.
void Candle::renderWithShadow()
{
	////Passing the shadow matrix the data it needs to squish steve into a shadow depending on the lights position.
	//shadow.generateShadowMatrix(shadowMatrix, lightPosition, floorVerts);

	////The candle will never be off the edge so I have not included the stencil for it...

	////glEnable(GL_STENCIL_TEST); {
	////	glStencilFunc(GL_ALWAYS, 1, 1); {
	////		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); {
	////			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); {
	////				glDisable(GL_DEPTH_TEST); {
	////					//Drawing stencil.
	////					glBegin(GL_QUADS); {
	////						glVertex3f(16, 14, 16);
	////						glVertex3f(16, 14, 21);
	////						glVertex3f(21, 14, 21);
	////						glVertex3f(21, 14, 16);
	////					}glEnd();
	////				}glEnable(GL_DEPTH_TEST);
	////			}glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	////		}glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	////	}glStencilFunc(GL_EQUAL, 1, 1);

	////Shadow Candle
	//glDisable(GL_TEXTURE_2D); {
	//	glDisable(GL_LIGHTING); {
	//		//Different color shadows for day and night.
	//		if (night) {
	//			glColor3f(0, 0, 0);
	//		}
	//		else {
	//			glColor3f(0.1f, 0.1f, 0.1f);
	//		}
	//		glPushMatrix(); {
	//			//Apply the shadow matrix.
	//			glMultMatrixf((GLfloat *)shadowMatrix);
	//			//Render the shadow.
	//			//render();
	//		}glPopMatrix();
	//		glColor3f(1.0f, 1.0f, 1.0f); 
	//	}glEnable(GL_LIGHTING);
	//}glEnable(GL_TEXTURE_2D);
	////}glDisable(GL_STENCIL_TEST);

	////Real Candle
	////render();
}
