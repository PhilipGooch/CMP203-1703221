#include "Block.h"



Block::Block(GLuint** textures, int type, Vector3 position)
	:textures_(textures), type_(type), wireFrame(false), position_(position)
{
	calculateVertexArrays();
}

Block::Block(GLuint** textures, GLuint** textures2, int type, Vector3 position)
	: textures_(textures), textures2_(textures2), type_(type), wireFrame(false), position_(position)
{
	calculateVertexArrays();
}


Block::~Block()
{
}

void Block::switchWireFrame()
{
	wireFrame = !wireFrame;
}

void Block::calculateVertexArrays()	
{
	//Index values for vertices, normals, texture coordinates and indicies.
	int v = 0;
	int n = 0;
	int t = 0;
	int in = 0;

	//Creating float to store the divisions cast to a float as I need to divide by this number and get a floating point number.
	float d = (float)div;

	//For each face.
	for (int k = 0; k < 6; k++) {
		v = 0;
		n = 0;
		t = 0;
		in = 0;
		//Front
		if (k == 0) {
			for (int i = 0; i <= div; i++) {
				for (int j = 0; j <= div; j++) {
					//Incrementing the index values when I use them. 
					vertices[k][v++] = position_.getX() + j / d;	
					vertices[k][v++] = position_.getY() + 1 - i / d;
					vertices[k][v++] = position_.getZ() + 1;
					normals[k][n++] = 0;
					normals[k][n++] = 0;
					normals[k][n++] = 1;
				}
			}
		}
		//Right
		else if (k == 1) {
			for (int i = 0; i <= div; i++) {
				for (int j = 0; j <= div; j++) {
					vertices[k][v++] = position_.getX() + 1;
					vertices[k][v++] = position_.getY() + 1 - i / d;
					vertices[k][v++] = position_.getZ() + 1 - j / d;
					normals[k][n++] = 1;
					normals[k][n++] = 0;
					normals[k][n++] = 0;
				}
			}
		}
		//Back
		else if (k == 2) {
			for (int i = 0; i <= div; i++) {
				for (int j = 0; j <= div; j++) {
					vertices[k][v++] = position_.getX() + 1 - j / d;
					vertices[k][v++] = position_.getY() + 1 - i / d;
					vertices[k][v++] = position_.getZ();
					normals[k][n++] = 0;
					normals[k][n++] = 0;
					normals[k][n++] = -1;
				}
			}
		}
		//Left
		else if (k == 3) {
			for (int i = 0; i <= div; i++) {
				for (int j = 0; j <= div; j++) {
					vertices[k][v++] = position_.getX();
					vertices[k][v++] = position_.getY() + 1 - i / d;
					vertices[k][v++] = position_.getZ() + j / d;
					normals[k][n++] = -1;
					normals[k][n++] = 0;
					normals[k][n++] = 0;
				}
			}
		}
		//Top
		else if (k == 4) {
			for (int i = 0; i <= div; i++) {
				for (int j = 0; j <= div; j++) {
					vertices[k][v++] = position_.getX() + j / d;
					vertices[k][v++] = position_.getY() + 1;
					vertices[k][v++] = position_.getZ() + i / d;
					normals[k][n++] = 0;
					normals[k][n++] = 1;
					normals[k][n++] = 0;
				}
			}
		}
		//Bottom
		else if (k == 5) {
			for (int i = 0; i <= div; i++) {
				for (int j = 0; j <= div; j++) {
					vertices[k][v++] = position_.getX() + j / d;
					vertices[k][v++] = position_.getY();
					vertices[k][v++] = position_.getZ() + 1 - i / d;
					normals[k][n++] = 0;
					normals[k][n++] = -1;
					normals[k][n++] = 0;
				}
			}
		}
		for (int i = 0; i <= div; i++) {
			for (int j = 0; j <= div; j++) {
				textureCoords[k][t++] = j / d;
				textureCoords[k][t++] = i / d;
			}
		}
		for (int i = 0; i < d; i++) {
			for (int j = 0; j < d; j++) {
				indices[k][in++] = j + i * (d + 1);
				indices[k][in++] = j + (i + 1) * (d + 1);
				indices[k][in++] = j + (i + 1) * (d + 1) + 1;
				indices[k][in++] = j + i * (d + 1) + 1;
			}
		}
	}
}

Vector3* Block::getPosition()
{
	return &position_;
}

void Block::render(bool* visibleFaces)
{
	//For each face.
	for (int i = 0; i < 6; i++) {
		//If the face is not visible, continue to the next face and do not render it().
		if (!visibleFaces[i]) {
			continue;
		}
		//If it is a "Leaf" or "Water" block, enable blending as these both use transparency.
		if (type_ == 7 || type_ == 9) {
			glEnable(GL_BLEND);
		}
		//If it is a "Water" block, disable lighting as it is used in reflection.
		if (type_ == 9) {
			glDisable(GL_LIGHTING);
		}
		//At a distance I want to blur the pixels.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		//Close up I want the pixels to be well defined.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//Using GL_REPEAT to get rid of issue with getting "seams" on the textures.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Rendering using vertex arrays.
		glEnableClientState(GL_VERTEX_ARRAY); {
			glEnableClientState(GL_NORMAL_ARRAY); {
				glEnableClientState(GL_TEXTURE_COORD_ARRAY); {
					//If it is an "Ore" block, apply material values to the texture to make it "shiny".
					if (type_ == 8) {
						GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
						GLfloat shininess[] = { 100 };
						GLfloat emission[] = { 0.5, 0.2, 0.2, 0.0 };
						glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
						glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
						glMaterialfv(GL_FRONT, GL_EMISSION, emission);
					}
					//If in wireFrame mode, do not bind a texture.
					if (wireFrame) {
						glBindTexture(GL_TEXTURE_2D, NULL);
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						glColor3f(0, 0, 0);
					}
					else {															//Consider only binding the texture if the texture before it is different... 
						glBindTexture(GL_TEXTURE_2D, *textures_[i]);				//Also I could render all the grass blocks first then rock for example... 
						glPolygonMode(GL_FRONT, GL_FILL);							//I could maybe even order them... 																
					}																//This would limit the amount of expensive glBindTexture() calls...

					glVertexPointer(3, GL_FLOAT, 0, vertices[i]);
					glNormalPointer(GL_FLOAT, 0, normals[i]);
					glTexCoordPointer(2, GL_FLOAT, 0, textureCoords[i]);

					glDrawElements(GL_QUADS, 4 * div * div, GL_UNSIGNED_SHORT, indices[i]);

				}glDisableClientState(GL_VERTEX_ARRAY);
			}glDisableClientState(GL_NORMAL_ARRAY);
		}glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//Reseting material values.
		if (type_ == 8) {
			GLfloat no_mat[] = { 0.0, 0.0, 0.0, 0.0 };
			glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);
			glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
			glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		}
	}
	//Resetting states.
	if (type_ == 9) {
		glEnable(GL_LIGHTING);
	}
	if (type_ == 7 || type_ == 9) {
		glDisable(GL_BLEND);
	}
	//If it is an "Ore" block I need to render it's "shell". "Ore" blocks are made up of two textures. One has a shiny material and one is like a stencil that has holes in it to see the "shiny" material underneath. 
	if (type_ == 8) {
		for (int i = 0; i < 6; i++) {
			if (!visibleFaces[i]) {
				continue;
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			//Enabling blending as it uses transparency.
			glEnable(GL_BLEND); {
				glEnableClientState(GL_VERTEX_ARRAY); {
					glEnableClientState(GL_NORMAL_ARRAY); {
						glEnableClientState(GL_TEXTURE_COORD_ARRAY); {

							if (wireFrame) {
								glBindTexture(GL_TEXTURE_2D, NULL);
								glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
								glColor3f(0, 0, 0);
							}
							else {
								glBindTexture(GL_TEXTURE_2D, *textures2_[i]);		//Consider only binding the texture if the texture before it is different. 
								glPolygonMode(GL_FRONT, GL_FILL);					//Also I could render all the grass blocks first then rock for example.
																					//I could maybe even order them. 
																					//This would limit the amount of expensive glBindTexture() calls.
							}
							glVertexPointer(3, GL_FLOAT, 0, vertices[i]);
							glNormalPointer(GL_FLOAT, 0, normals[i]);
							glTexCoordPointer(2, GL_FLOAT, 0, textureCoords[i]);

							glDrawElements(GL_QUADS, 4 * div * div, GL_UNSIGNED_SHORT, indices[i]);

						}glDisableClientState(GL_VERTEX_ARRAY);
					}glDisableClientState(GL_NORMAL_ARRAY);
				}glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}glDisable(GL_BLEND);
		}
	}
}

//Not used...
//bool Block::getEnclosed()
//{
//	return enclosed_;
//}

//Not used...
//void Block::setEnclosed(bool enclosed)
//{
//	enclosed_ = enclosed;
//}
