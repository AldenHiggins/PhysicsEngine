#include "PlayerControls.h"
#include "MathDataTypes.h"
#include <iostream>

using namespace PhysicsDemo;

// Update player position as needed given the duration of the past frame
void PlayerController::update(float deltaTime)
{
	Vector3 forward = getForward();
	Vector3 right = forward.vectorProduct(Vector3(0.0f, 1.0f, 0.0f));
	
	// Zero out the y movement
	forward[1] = 0.0f;
	right[1] = 0.0f;

	if (wPressed)
	{
		position += forward * speed;
	}

	if (sPressed)
	{
		position -= forward * speed;
	}

	if (aPressed)
	{
		position -= right * speed;
	}

	if (dPressed)
	{
		position += right * speed;
	}
}

// Controls
void PlayerController::keyCheck(unsigned char key)
{
	switch (key)
	{
	case 'w':
	{
		wPressed = true;
		break;
	}
	case 's':
	{
		sPressed = true;
		break;
	}
	case 'a':
	{
		aPressed = true;
		break;
	}
	case 'd':
	{
		dPressed = true;
		break;
	}
	}
}

// Controls
void PlayerController::keyUpCheck(unsigned char key)
{
	switch (key)
	{
	case 'w':
	{
		wPressed = false;
		break;
	}
	case 's':
	{
		sPressed = false;
		break;
	}
	case 'a':
	{
		aPressed = false;
		break;
	}
	case 'd':
	{
		dPressed = false;
		break;
	}
	}
}

// Getters and setters
Quaternion PlayerController::getRotation()
{
	return rotation;
}

void PlayerController::setRotation(Quaternion newRotation)
{
	rotation = newRotation;
}

Vector3 PlayerController::getForward()
{
	// Hacky from euler creation...but it works
	//Quaternion rotation = Quaternion::fromEuler(yaw, 0.0f, -pitch);

	Matrix3 rotMatrix;
	rotMatrix.setOrientation(rotation);

	return rotMatrix.transform(Vector3(0.0f, 0.0f, 1.0f));
}


real PlayerController::getSpeed()
{
	return speed;
}

void PlayerController::setSpeed(real newSpeed)
{
	speed = newSpeed;
}

Vector3 PlayerController::getPosition()
{
	return position;
}
void PlayerController::setPosition(Vector3 newPosition)
{
	position = newPosition;
}

real PlayerController::getYaw()
{
	return yaw;
}

void PlayerController::setYaw(real newYaw)
{
	yaw = newYaw;
}

real PlayerController::getPitch()
{
	return pitch;
}
void PlayerController::setPitch(real newPitch)
{
	pitch = newPitch;
}