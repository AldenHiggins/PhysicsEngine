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

	// Update velocity based on the acceleration
	velocity.addScaledVector(acceleration, timeStep);
}

// Display this particle
void Particle::display()
{
	// Don't display a dead particle
	if (timeAliveSoFar > lifeTime)
	{
		return;
	}
	glBegin(GL_QUADS);
	// Set the color for the particle
	glColor3f(color[0], color[1], color[2]);
	// Now render the particle
	glVertex3f(position[0] - size, position[1] - size, position[2]);
	glVertex3f(position[0] + size, position[1] - size, position[2]);
	glVertex3f(position[0] + size, position[1] + size, position[2]);
	glVertex3f(position[0] - size, position[1] + size, position[2]);
	glEnd();
}

// Display this circular particle
void CircleParticle::display()
{
	std::cout << "Position: " << position[0] << " " << position[1] << " " << position[2] << std::endl;
	glColor3f(color[0], color[1], color[2]);
	//glTranslatef(position[0], position[1], position[2]);
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glutSolidSphere(size, 20.0, 20.0);
	glPopMatrix();
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
