#include "PlayerControls.h"
#include "MathDataTypes.h"

using namespace PhysicsDemo;

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