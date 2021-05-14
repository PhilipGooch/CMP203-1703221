#include "Box.h"


Box::Box(GLuint* texture_, float width_, float height_, float depth_, float size)
{
	texture = texture_;

	//Scaling the model down to be at the same scale as my blocks.
	width = width_ * size;
	height = height_ * size;
	depth = depth_ * size;
	calculateCoords();
}

Box::~Box()
{
}

void Box::render()
{
	//Using GL_REPEAT to solve an issue with gettin seams on my boxes.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//I found these parameters to give the nicest effect.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, *texture);
	//Setting vertices, normals and texture coordinates and drawing quads for each face of the box.
	glBegin(GL_QUADS);
	//Front
	glNormal3f(0, 0, 1);
	glTexCoord2f(texturePoints[7].x, texturePoints[7].y);
	glVertex3f(-width / 2, height / 2, depth / 2);
	glNormal3f(0, 0, 1);
	glTexCoord2f(texturePoints[10].x, texturePoints[10].y);
	glVertex3f(-width / 2, -height / 2, depth / 2);
	glNormal3f(0, 0, 1);
	glTexCoord2f(texturePoints[11].x, texturePoints[11].y);
	glVertex3f(width / 2, -height / 2, depth / 2);
	glNormal3f(0, 0, 1);
	glTexCoord2f(texturePoints[8].x, texturePoints[8].y);
	glVertex3f(width / 2, height / 2, depth / 2);
	//Right
	glNormal3f(1, 0, 0);
	glTexCoord2f(texturePoints[8].x, texturePoints[8].y);
	glVertex3f(width / 2, height / 2, depth / 2);
	glNormal3f(1, 0, 0);
	glTexCoord2f(texturePoints[9].x, texturePoints[9].y);
	glVertex3f(width / 2, -height / 2, depth / 2);
	glNormal3f(1, 0, 0);
	glTexCoord2f(texturePoints[5].x, texturePoints[5].y);
	glVertex3f(width / 2, -height / 2, -depth / 2);
	glNormal3f(1, 0, 0);
	glTexCoord2f(texturePoints[4].x, texturePoints[4].y);
	glVertex3f(width / 2, height / 2, -depth / 2);
	//Back
	glNormal3f(0, 0, -1);
	glTexCoord2f(texturePoints[4].x, texturePoints[4].y);
	glVertex3f(width / 2, height / 2, -depth / 2);
	glNormal3f(0, 0, -1);
	glTexCoord2f(texturePoints[1].x, texturePoints[1].y);
	glVertex3f(width / 2, -height / 2, -depth / 2);
	glNormal3f(0, 0, -1);
	glTexCoord2f(texturePoints[0].x, texturePoints[0].y);
	glVertex3f(-width / 2, -height / 2, -depth / 2);
	glNormal3f(0, 0, -1);
	glTexCoord2f(texturePoints[3].x, texturePoints[3].y);
	glVertex3f(-width / 2, height / 2, -depth / 2);
	//Left
	glNormal3f(-1, 0, 0);
	glTexCoord2f(texturePoints[3].x, texturePoints[3].y);
	glVertex3f(-width / 2, height / 2, -depth / 2);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(texturePoints[2].x, texturePoints[2].y);
	glVertex3f(-width / 2, -height / 2, -depth / 2);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(texturePoints[6].x, texturePoints[6].y);
	glVertex3f(-width / 2, -height / 2, depth / 2);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(texturePoints[7].x, texturePoints[7].y);
	glVertex3f(-width / 2, height / 2, depth / 2);
	//Top
	glNormal3f(0, 1, 0);
	glTexCoord2f(texturePoints[3].x, texturePoints[3].y);
	glVertex3f(-width / 2, height / 2, -depth / 2);
	glNormal3f(0, 1, 0);
	glTexCoord2f(texturePoints[7].x, texturePoints[7].y);
	glVertex3f(-width / 2, height / 2, depth / 2);
	glNormal3f(0, 1, 0);
	glTexCoord2f(texturePoints[8].x, texturePoints[8].y);
	glVertex3f(width / 2, height / 2, depth / 2);
	glNormal3f(0, 1, 0);
	glTexCoord2f(texturePoints[4].x, texturePoints[4].y);
	glVertex3f(width / 2, height / 2, -depth / 2);
	//Bottom
	glNormal3f(0, -1, 0);
	glTexCoord2f(texturePoints[10].x, texturePoints[10].y);
	glVertex3f(-width / 2, -height / 2, depth / 2);
	glNormal3f(0, -1, 0);
	glTexCoord2f(texturePoints[12].x, texturePoints[12].y);
	glVertex3f(-width / 2, -height / 2, -depth / 2);
	glNormal3f(0, -1, 0);
	glTexCoord2f(texturePoints[13].x, texturePoints[13].y);
	glVertex3f(width / 2, -height / 2, -depth / 2);
	glNormal3f(0, -1, 0);
	glTexCoord2f(texturePoints[11].x, texturePoints[11].y);
	glVertex3f(width / 2, -height / 2, depth / 2);
	glEnd();
}

void Box::calculateCoords()
{
	//Images must be in this format with top in the middle of the T, front, right, back and left going counter clockwise around the middle starting from the bottom, oriented with their top around the top face, and bottom at the bottom of the T.

	//    --textureWidth--
	//
	//         0____1		     
	//         |    |		     |
	//    2___3|____|4___5       |
	//    |    |    |    |       |
	//    |____|____|____|  textureHeight
	//    6   7|    |8	 9		 |
	//         |____|            |
	//       10|    |11          |
	//         |____|            |
	//       12      13

	//The height of the right and left faces plus the width of the top face.
	float textureWidth = height * 2 + width;
	
	//The height of the front and back faces plus the depth of the top face plus the depth of the bottom face.
	float textureHeight = height * 2 + depth * 2;		

	//Storing the x coordinates of each of the columns' edges.
	std::vector<float> Xcoords{ 0, height, height + width, height * 2 + width };

	//Storing the y coordinates of each of the rows' edges.
	std::vector<float> Ycoords{ 0, height, height + depth, height * 2 + depth, height * 2 + depth * 2 };

	//Refer to the diagram above to see which texture point I am setting.
	texturePoints[0].x = height / textureWidth;
	texturePoints[0].y = 0;

	texturePoints[1].x = (height + width) / textureWidth;
	texturePoints[1].y = 0;

	texturePoints[2].x = 0;
	texturePoints[2].y = height / textureHeight;

	texturePoints[3].x = height / textureWidth;
	texturePoints[3].y = height / textureHeight;

	texturePoints[4].x = (height + width) / textureWidth;
	texturePoints[4].y = height / textureHeight;

	texturePoints[5].x = 1;
	texturePoints[5].y = height / textureHeight;

	texturePoints[6].x = 0;
	texturePoints[6].y = (height + depth) / textureHeight;

	texturePoints[7].x = height / textureWidth;
	texturePoints[7].y = (height + depth) / textureHeight;

	texturePoints[8].x = (height + width) / textureWidth;
	texturePoints[8].y = (height + depth) / textureHeight;

	texturePoints[9].x = 1;
	texturePoints[9].y = (height + depth) / textureHeight;

	texturePoints[10].x = height / textureWidth;
	texturePoints[10].y = (height * 2 + depth) / textureHeight;

	texturePoints[11].x = (height + width) / textureWidth;
	texturePoints[11].y = (height * 2 + depth) / textureHeight;

	texturePoints[12].x = height / textureWidth;
	texturePoints[12].y = 1;

	texturePoints[13].x = (height + width) / textureWidth;
	texturePoints[13].y = 1;
}

float Box::getWidth() { return width; }

float Box::getHeight() { return height; }

float Box::getDepth() { return depth; }
