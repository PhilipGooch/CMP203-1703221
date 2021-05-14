#include "PC.h"

//Passing the parameters up to the base class NPC.
PC::PC(Input* input_, int skin_, Model models_[3]) : NPC(input_, skin_, models_)
{
	//The speed the character runs.
	speed = 0.6f;
	//For the heading of velocity vector.			
	direction = 0;
	//The direction the player is oriented while strafing.
	strafeDirection = 0;
	models = models_;
	//passing the player model it's tool index.
	model->setToolIndex(toolIndex);
}

PC::~PC()
{
	delete model;
}

void PC::setAngle(float angle_) { angle = angle_; }

void PC::setPerspective(int perspective) {
	perspective_ = perspective;
}

float PC::getAngle() { return angle; }

Vector3 PC::getPosition() { return position; }

MyModel * PC::getModel() { return model; }

void PC::AI(float dt)
{
	return;
}

void PC::handleInput(float dt)
{
	//+0.0000001 so that velocity is never zero which causes division by 0 error.	
	if (dt == 0) {
		dt += 0.000001f;		
	}

	//Cycling through tools.
	if (input->isKeyDown('f')) {
		input->SetKeyUp('f');
		toolIndex++;
		if (toolIndex == 3) {
			toolIndex = 0;
		}
		model->setToolIndex(toolIndex);
	}

	//First person.
	if (perspective_ == 0) {
		if (input->isKeyDown('w') && input->isKeyDown('a')) {
			direction = 305;
		}
		else if (input->isKeyDown('w') && input->isKeyDown('d')) {
			direction = 45;
		}
		else if (input->isKeyDown('s') && input->isKeyDown('a')) {
			direction = 225;
		}
		else if (input->isKeyDown('s') && input->isKeyDown('d')) {
			direction = 135;
		}
		else if (input->isKeyDown('w')) {
			direction = 0;
		}
		else if (input->isKeyDown('s')) {
			direction = 180;
		}
		else if (input->isKeyDown('a')) {
			direction = 270;
		}
		else if (input->isKeyDown('d')) {
			direction = 90;
		}
		//This method of movement means the player will travel the same distance despite diagonals.
		if (input->isKeyDown('w') || input->isKeyDown('s') || input->isKeyDown('a') || input->isKeyDown('d')) {
			velocity.setX(cosf((angle - 90 + direction) * PI / 180) * speed * dt);
			velocity.setZ(sinf((angle - 90 + direction) * PI / 180) * speed * dt);
			walking = true;
		}
		else {
			direction = 0;
			strafeDirection = 0;
			velocity.setX(0);
			velocity.setZ(0);
			walking = false;
		}
	}
	//Third person
	//Here I was going for the controls used in World of Warcraft. 
	else if (perspective_ == 1) {
		//If the right mouse button is pressed the player's angle is determined by the mouse and 'a' and 'd' are used for strafing.
		if (input->rightMouseButtonPressed()) {
			if (input->isKeyDown('w') && (input->isKeyDown('a') || input->isKeyDown('q'))) {
				direction = 305;
				strafeDirection = 305;
			}
			else if (input->isKeyDown('w') && (input->isKeyDown('d') || input->isKeyDown('e'))) {
				direction = 45;
				strafeDirection = 45;
			}
			else if (input->isKeyDown('s') && (input->isKeyDown('a') || input->isKeyDown('q'))) {
				direction = 225;
				strafeDirection = 45;
			}
			else if (input->isKeyDown('s') && (input->isKeyDown('d') || input->isKeyDown('e'))) {
				direction = 135;
				strafeDirection = 305;
			}
			else if (input->isKeyDown('w')) {
				direction = 0;
				strafeDirection = 0;
			}
			else if (input->isKeyDown('s')) {
				direction = 180;
				strafeDirection = 0;
			}
			else if (input->isKeyDown('a') || input->isKeyDown('q')) {
				direction = 270;
				strafeDirection = 270;
			}
			else if (input->isKeyDown('d') || input->isKeyDown('e')) {
				direction = 90;
				strafeDirection = 90;
			}
		}
		//If the right mouse button is not pressed, 'a' and 'd' rotate the player and 'q' and 'e' are used for strafing.
		else if (!input->rightMouseButtonPressed()) {
			if (input->isKeyDown('a')) {
				angle -= rotateSpeed * dt;
			}
			if (input->isKeyDown('d')) {
				angle += rotateSpeed * dt;
			}
			if (input->isKeyDown('w') && input->isKeyDown('q')) {
				direction = 305;
				strafeDirection = 305;
			}
			else if (input->isKeyDown('w') && input->isKeyDown('e')) {
				direction = 45;
				strafeDirection = 45;
			}
			else if (input->isKeyDown('s') && input->isKeyDown('q')) {
				direction = 225;
				strafeDirection = 45;
			}
			else if (input->isKeyDown('s') && input->isKeyDown('e')) {
				direction = 135;
				strafeDirection = 305;
			}
			else if (input->isKeyDown('w')) {
				direction = 0;
				strafeDirection = 0;
			}
			else if (input->isKeyDown('s')) {
				direction = 180;
				strafeDirection = 0;
			}
			else if (input->isKeyDown('q')) {
				direction = 270;
				strafeDirection = 270;
			}
			else if (input->isKeyDown('e')) {
				direction = 90;
				strafeDirection = 90;
			}
		}
		//If both mouse buttons are pressed, the player walks and it's angle is controlled by the mouse.
		if (input->isKeyDown('w') || input->isKeyDown('s') || input->isKeyDown('q') || input->isKeyDown('e') || (input->rightMouseButtonPressed() && (input->isKeyDown('a') || input->isKeyDown('d'))) || (input->rightMouseButtonPressed() && input->leftMouseButtonPressed())) {
			//Setting the velocity of the player.
			velocity.setX(cosf((angle - 90 + direction) * PI / 180) * speed * dt);
			velocity.setZ(sinf((angle - 90 + direction) * PI / 180) * speed * dt);
			//Boolean used to tell the model to animate or not.
			walking = true;
		}
		//If the player is not walking, reset the direction variables and set the velocity to zero.
		else {
			direction = 0;
			strafeDirection = 0;
			velocity.setX(0);
			velocity.setZ(0);
			walking = false;
		}
		//Functionality to change the model associated with the player.
		/*if (input->isKeyDown('J')) {
			input->SetKeyUp('J');
			delete model;
			model = new Pig();
		}
		if (input->isKeyDown('K')) {
			input->SetKeyUp('K');
			delete model;
			model = new Cow();
		}
		if (input->isKeyDown('L')) {
			input->SetKeyUp('L');
			delete model;
			model = new Steve(models);
		}*/
	}
	//Setting the direction (angle) of the model.
	model->setDirection(direction);
	model->setStrafeDirection(strafeDirection);
}



