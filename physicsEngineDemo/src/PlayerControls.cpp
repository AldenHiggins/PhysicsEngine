#include "PlayerControls.h"
#include "MathDataTypes.h"
#include <iostream>

using namespace PhysicsDemo;

// Controls
void PlayerController::keyCheck(unsigned char key)
{
	// Hacky from euler creation...but it works
	PhysicsEngine::Quaternion rotation = PhysicsEngine::Quaternion::fromEuler(yaw, 0.0f, -pitch);

	PhysicsEngine::Matrix3 rotMatrix;
	rotMatrix.setOrientation(rotation);

	PhysicsEngine::Vector3 forward = rotMatrix.transform(PhysicsEngine::Vector3(0.0f, 0.0f, 1.0f));
	PhysicsEngine::Vector3 right = forward.vectorProduct(PhysicsEngine::Vector3(0.0f, 1.0f, 0.0f));

	std::cout << "Forward: " << forward[0] << " " << forward[1] << " " << forward[2] << std::endl;

	switch (key)
	{
	case 'w':
	{
		position += forward * speed;
		break;
	}
	case 's':
	{
		position -= forward * speed;
		break;
	}
	case 'a':
	{
		position -= right * speed;
		break;
	}
	case 'd':
	{
		position += right * speed;
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