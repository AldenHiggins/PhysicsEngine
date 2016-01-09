#include "MathDataTypes.h"
#include "Particle.h"
#include <iostream>

using namespace PhysicsEngine;

// Integrate the particle forward in time by the timestep
void Particle::integrate(real timeStep)
{
	timeAliveSoFar += timeStep;

	// Don't integrate the particle before's it's done getting ready
	if (timeAliveSoFar < startupTime)
	{
		return;
	}

	// Don't update a dead particle
	if (timeAliveSoFar > lifeTime && !isDead)
	{
		isDead = true;
		return;
	}
	// Update the position based on the particle's velocity
	position.addScaledVector(velocity, timeStep);

	// Update velocity based on the acceleration
	velocity.addScaledVector(acceleration, timeStep);
}

// Getters and setters for particle data
Vector3 Particle::getPosition()
{
	return position;
}

void Particle::setPosition(Vector3 newPosition)
{
	position = newPosition;
}

Vector3 Particle::getVelocity()
{
	return velocity;
}

void Particle::setVelocity(Vector3 newVelocity)
{
	velocity = newVelocity;
}

Vector3 Particle::getAcceleration()
{
	return acceleration;
}

void Particle::setAcceleration(Vector3 newAcceleration)
{
	acceleration = newAcceleration;
}

real Particle::getLifeTime()
{
	return lifeTime;
}

void Particle::setStartupTime(real startupTimeInput)
{
	startupTime = startupTimeInput;
}

void Particle::setLifeTime(real newLifetime)
{
	lifeTime = newLifetime + startupTime;
}

bool Particle::getIsDead()
{
	return isDead;
}
