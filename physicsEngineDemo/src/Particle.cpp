#include "MathDataTypes.h"
#include "Particle.h"
#include <iostream>

using namespace PhysicsEngine;

// Integrate the particle forward in time by the timestep
void Particle::integrate(real timeStep)
{
	std::cout << "Velocity: " << velocity[0] << " " << velocity[1] << " " << velocity[2] << std::endl;
	// Update the position based on the particle's velocity
	position.addScaledVector(velocity, timeStep);
	std::cout << "Velocity after: " << velocity[0] << " " << velocity[1] << " " << velocity[2] << std::endl;
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

real Particle::getSize()
{
	return size;
}

void Particle::setSize(real newSize)
{
	size = newSize;
}

Vector3 Particle::getColor()
{
	return color;
}

void Particle::setColor(Vector3 newColor)
{
	color = newColor;
}
