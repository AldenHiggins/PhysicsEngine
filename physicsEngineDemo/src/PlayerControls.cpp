#include "PlayerControls.h"
#include "MathDataTypes.h"
#include <iostream>

using namespace PhysicsDemo;

// Update player position as needed given the duration of the past frame
void PlayerController::update(float deltaTime)
{
	// Hacky from euler creation...but it works
	PhysicsEngine::Quaternion rotation = PhysicsEngine::Quaternion::fromEuler(yaw, 0.0f, -pitch);

	PhysicsEngine::Matrix3 rotMatrix;
	rotMatrix.setOrientation(rotation);

	PhysicsEngine::Vector3 forward = rotMatrix.transform(PhysicsEngine::Vector3(0.0f, 0.0f, 1.0f));
	PhysicsEngine::Vector3 right = forward.vectorProduct(PhysicsEngine::Vector3(0.0f, 1.0f, 0.0f));

	//std::cout << "Forward: " << forward[0] << " " << forward[1] << " " << forward[2] << std::endl;

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
PhysicsEngine::real PlayerController::getSpeed()
{
	return speed;
}

void PlayerController::setSpeed(PhysicsEngine::real newSpeed)
{
	speed = newSpeed;
}

PhysicsEngine::Vector3 PlayerController::getPosition()
{
	return position;
}
void PlayerController::setPosition(PhysicsEngine::Vector3 newPosition)
{
	position = newPosition;
}

PhysicsEngine::real PlayerController::getYaw()
{
	return yaw;
}

void PlayerController::setYaw(PhysicsEngine::real newYaw)
{
	yaw = newYaw;
}

PhysicsEngine::real PlayerController::getPitch()
{
	return pitch;
}
void PlayerController::setPitch(PhysicsEngine::real newPitch)
{
	pitch = newPitch;
}