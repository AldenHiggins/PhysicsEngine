#include "MathDataTypes.h"
#include "Particle.h"
#include <GL/glut.h>
#include <iostream>

using namespace PhysicsEngine;

// Integrate the particle forward in time by the timestep
void Particle::integrate(real timeStep)
{
	timeAliveSoFar += timeStep;
	// Update the position based on the particle's velocity
	position.addScaledVector(velocity, timeStep);
}

// Display this particle
void Particle::display()
{
	// Don't display a dead particle
	if (timeAliveSoFar > lifeTime)
	{
		return;
	}
	// Set the color for the particle
	glColor3f(color[0], color[1], color[2]);
	// Now render the particle
	glVertex3f(position[0] - size, position[1] - size, position[2]);
	glVertex3f(position[0] + size, position[1] - size, position[2]);
	glVertex3f(position[0] + size, position[1] + size, position[2]);
	glVertex3f(position[0] - size, position[1] + size, position[2]);
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

real Particle::getLifeTime()
{
	return lifeTime;
}

void Particle::setLifeTime(real newLifetime)
{
	lifeTime = newLifetime;
}

bool Particle::getIsDead()
{

	return isDead;
}
