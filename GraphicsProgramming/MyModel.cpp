#include "MyModel.h"



MyModel::MyModel()
{
	//When I created my custom models I made each minecraft pixel one unit in size. I therefor need to scale down the models by 1/16 to get its exact scale.
	size = 0.0625;
}

MyModel::~MyModel()
{
}

void MyModel::animate(float dt)
{
	//Rotating the limbs.
	for (int i = 0; i < 4; i++) {
		limbAngles[i] += limbDirections[i] * limbSpeed * dt;
		//If the limb reaches its maximum angle in either direction, swap its direction.
		if (limbAngles[i] > 45) {
			limbAngles[i] = 45;
			limbDirections[i] = -limbDirections[i];
		}
		if (limbAngles[i] < -45) {
			limbAngles[i] = -45;
			limbDirections[i] = -limbDirections[i];
		}
	}
}

void MyModel::resetAnimation()
{
	//Setting all limb angles to zero when it stops walking.
	for (int i = 0; i < 4; i++) {
		limbAngles[i] = 0;
	}
}

void MyModel::render()
{
}

void MyModel::render(int light)
{
}

void MyModel::renderWithShadow()
{
}

Box* MyModel::getBody() { return body; }
Box* MyModel::getHead() { return head; }
Box* MyModel::getLeftArm() { return leftArm; }
Box* MyModel::getRightArm() { return rightArm; }
Box* MyModel::getLeftLeg() { return leftLeg; }
Box* MyModel::getRightLeg() { return rightLeg; }
Box* MyModel::getNose() { return nose; }
Box* MyModel::getLeftHorn() { return leftHorn; }
Box* MyModel::getRightHorn() { return rightHorn; }
Box* MyModel::getUdders() { return udders; }

Vector3& MyModel::getBodyOffset() { return bodyOffset; }
Vector3& MyModel::getHeadOffset() { return headOffset; }
Vector3& MyModel::getLeftArmOffset() { return leftArmOffset; }
Vector3& MyModel::getRightArmOffset() { return rightArmOffset; }
Vector3& MyModel::getLeftLegOffset() { return leftLegOffset; }
Vector3& MyModel::getRightLegOffset() { return rightLegOffset; }
Vector3& MyModel::getNoseOffset() { return noseOffset; }
Vector3& MyModel::getLeftHornOffset() { return leftHornOffset; }
Vector3& MyModel::getRightHornOffset() { return rightHornOffset; }
Vector3& MyModel::getUddersOffset() { return uddersOffset; }

float MyModel::getEyesY()
{
	return eyesY;
}

float* MyModel::getlimbAngles() { return limbAngles; }

void MyModel::setToolIndex(int toolIndex_)
{
	toolIndex = toolIndex_;
}

void MyModel::setHat(bool hat_)
{
	hat = hat_;
}

void MyModel::setPosition(float x, float y, float z)
{
	position.setX(x);
	position.setY(y);
	position.setZ(z);
}

void MyModel::setAngle(float angle_)
{
	angle = angle_;
}

void MyModel::setDirection(float direction_)
{
	direction = direction_;
}

void MyModel::setStrafeDirection(float strafeDirection_)
{
	strafeDirection = strafeDirection_;
}

void MyModel::setLightPosition(float x, float y, float z)
{
	lightPosition[0] = x;
	lightPosition[1] = y;
	lightPosition[2] = z;
	lightPosition[3] = 1;
}

void MyModel::setNight(bool night_)
{
	night = night_;
}

void MyModel::setStencil(std::vector<float>* stencil_)
{
	stencil = stencil_;
}


