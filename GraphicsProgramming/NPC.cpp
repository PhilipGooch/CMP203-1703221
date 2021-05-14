#include "NPC.h"


NPC::NPC(Input* input_, int skin_, Model models[3])
{
	input = input_;
	skin = skin_;

	speed = 0.35f;
	rotateSpeed = 20.f;
	roamRadius = 6;
	walking = false;

	//Storing a copy of the NPC's original position.
	spawnPosition = position;

	//If it is not the player give the NPC a random angle.
	//Creating the appropriate custom model.
	switch (skin) {
	case 0:
		angle = 0;
		model = new Steve(models);
		break;
	case 1:
		angle = rand() % 360;
		model = new Pig();
		break;
	case 2:
		angle = rand() % 360;
		model = new Cow();
		break;
	case 3:
		model = new Candle();
		break;
	}
}


NPC::~NPC()
{
}

//Sets the angle and passes it down to MyModel.
void NPC::setAngle(float angle_) 
{ 
	angle = angle_; 
	model->setAngle(angle);			
}

float NPC::getAngle() { return angle; }

Vector3 NPC::getPosition() { return position; }

void NPC::setPosition(float x, float y, float z)
{
	position.setX(x);
	position.setY(y);
	position.setZ(z);
}

void NPC::setPerspective(int pointOfView_) { return; }

MyModel* NPC::getModel() { return model; }

void NPC::AI(float dt)
{
	if (dt == 0) {
		dt += 0.000001f;		//+0.0000001 so that velocity is never zero which causes division by 0 error.	
	}
	if (dt > 1) {
		return;
	}
	walking = true;
	//Setting the velocity of the NPC 
	velocity.setX(cos(angle * PI / 180.f) * speed * dt);
	velocity.setZ(sin(angle * PI / 180.f) * speed * dt);
	//Creating a vector from the current position to the spawn position.
	Vector3 toSpawn = spawnPosition - position;
	//Finding the dot product of the vecocity vector and the to spawn vector.
	float dot = velocity.dot(toSpawn);
	float velocityMagnitude = velocity.magnitude();
	float toSpawnMagnitude = toSpawn.magnitude();
	//Calculting the determinant.
	float determinant = velocityMagnitude * toSpawnMagnitude;
	float brackets = dot / determinant;
	if (brackets > 1.f) {
		brackets = 1.f;
	}
	if (brackets < -1.f) {
		brackets = -1.f;
	}
	//Calculating the angle.
	float radians = acos(brackets);
	float theta = radians * 180.f / PI;
	//Using the square of the magnitude of the vector and the radius as square rooting is very expensive.
	float toSpawnMagnitudeSquared = (toSpawn.getX()) * (toSpawn.getX()) + (toSpawn.getZ()) * (toSpawn.getZ());
	float roamRadiusSquared = roamRadius * roamRadius;
	//If it walks out of its roaming circle, set its angle to walk to the spawn position plus or minus a random offset.
	if (toSpawnMagnitudeSquared > roamRadiusSquared) {
		angle += theta + (rand() % 61) - 30;
		if (angle >= 360.f) {
			angle -= 360.f;
		}
		if (angle < 0.f) {
			angle += 360.f;
		}
	}
}

void NPC::handleInput(float dt)
{
	return;
}

void NPC::update(float dt)
{
	model->setPosition(position.getX(), position.getY(), position.getZ());
	//If the NPC is the candle, do not do anything.		//Candle should not be an NPC...!
	if (skin != 3) {
		model->setAngle(angle);
		AI(dt);
		position += velocity;
		//Animate the model.
		if (walking) {
			model->animate(dt);
		}
		else {
			model->resetAnimation();
		}
	}
}

void NPC::render()
{
	model->render();
}

void NPC::render(int light)
{
	model->render(light);
}

void NPC::renderWithShadow()
{
	model->renderWithShadow();
}

void NPC::setSpawnPosition()
{
	spawnPosition = position;
}

void NPC::setLightPosition(float x, float y, float z)
{
	model->setLightPosition(x, y, z);
}

void NPC::setNight(bool night_)
{
	model->setNight(night_);
}

void NPC::setStencil(std::vector<float>* stencil)
{
	model->setStencil(stencil);
}

void NPC::setHat(bool hat)
{
	model->setHat(hat);
}
