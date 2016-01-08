#include "MathDataTypes.h"
#include "RenderParticles.h"
#include <GL/glut.h>
#include <iostream>

using namespace PhysicsDemo;

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
		onDeath();
		return;
	}
	// Update the position based on the particle's velocity
	position.addScaledVector(velocity, timeStep);

	// Update velocity based on the acceleration
	velocity.addScaledVector(acceleration, timeStep);
}

// Display this particle
void Particle::display()
{
	// Don't display a dead particle
	if (isDead)
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

	glColor4f(1, 1, 1, 1);
}

// Empty on death function
void Particle::onDeath(){}

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

// Display this circular particle
void CircleParticle::display()
{
	// Don't display the particle before's it's done getting ready
	if (timeAliveSoFar < startupTime)
	{
		return;
	}

	// Don't display a dead particle
	if (timeAliveSoFar > lifeTime)
	{
		return;
	}
	glColor3f(color[0], color[1], color[2]);
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glutSolidSphere(size, 20.0, 20.0);
	glPopMatrix();
}

// Integrate the firework particle
void FireworkParticle::integrate(real timeStep)
{
	if (isDead)
	{
		if (lives <= 0)
		{
			return;
		}

		for (int particleIndex = 0; particleIndex < 5; particleIndex++)
		{
			deathParticles[particleIndex]->integrate(timeStep);
		}
	}
	else
	{
		Particle::integrate(timeStep);
	}
}

// Show the firework
void FireworkParticle::display()
{
	// If the firework is dead display it's child particles if it has them
	if (isDead)
	{
		// Check to see if this firework has any more lives
		if (lives <= 0)
		{
			return;
		}
		for (int particleIndex = 0; particleIndex < 5; particleIndex++)
		{
			deathParticles[particleIndex]->display();
		}
	}
	else
	{
		CircleParticle::display();
	}
}

// Generate child particles of this firework
void FireworkParticle::onDeath()
{
	// Don't generate any more particles if this firework is out of lives
	if (lives <= 0)
	{
		return;
	}
	// Create child particles for this firework when it "detonates"
	for (int particleIndex = 0; particleIndex < 5; particleIndex++)
	{
		deathParticles[particleIndex] = CreateParticle::createFireWorkParticle(0.4f, .1f, position, color, false, lives - 1);
	}
}

// Generate a new circular particle
Particle* CreateParticle::createFireWorkParticle(real speed, real size, Vector3 position, Vector3 color, bool originalFirework, int lives)
{
	FireworkParticle *newParticle = new FireworkParticle(lives);
	real xVelocity = ((real)((rand() % 800) - 400)) / 100;
	real yVelocity = ((real)((rand() % 800) - 0)) / 100;
	real zVelocity = ((real)((rand() % 800) - 400)) / 100;

	xVelocity *= speed;
	yVelocity *= speed * 2;
	zVelocity *= speed;

	// Make the original firework follow a predictable path
	if (originalFirework)
	{
		xVelocity = 0;
		yVelocity = 8.0f * speed;
		zVelocity = 0;
		// Give the particle a startup time for GIF recording purposes
		newParticle->setStartupTime(5.0f);
	}

	newParticle->setVelocity(Vector3(xVelocity, yVelocity, zVelocity));
	// Add gravity onto this circular particle
	newParticle->setAcceleration(Vector3(0.0f, -3.81f, 0.0f));
	newParticle->setPosition(position);
	newParticle->setColor(color);
	newParticle->setSize(size);
	newParticle->setLifeTime(1.0f);
	return newParticle;
}

// Generate a new circular particle
Particle* CreateParticle::createCircularParticle(real speed, real size, Vector3 color)
{
	CircleParticle *newParticle = new CircleParticle();
	real xVelocity = ((real)((rand() % 200) - 100)) / 100;
	real yVelocity = 30.0f;
	real zVelocity = ((real)((rand() % 200) - 100)) / 100;
	xVelocity *= speed;
	yVelocity *= speed;
	zVelocity *= speed;
	newParticle->setVelocity(Vector3(xVelocity, yVelocity, zVelocity));
	// Add gravity onto this circular particle
	newParticle->setAcceleration(Vector3(0.0f, -9.81f, 0.0f));
	newParticle->setPosition(Vector3(0.0f, 4.0f, 6.0f));
	newParticle->setColor(color);
	newParticle->setSize(size);
	newParticle->setLifeTime(2.0f);
	return newParticle;
}

// Generate a new particle
Particle* CreateParticle::createParticle(real speed, real size, Vector3 color)
{
	Particle *newParticle = new Particle();
	real xVelocity = ((real)((rand() % 200) - 100)) / 100;
	real yVelocity = ((real)((rand() % 200) - 100)) / 100;
	real zVelocity = ((real)((rand() % 200) - 100)) / 100;
	xVelocity *= speed;
	yVelocity *= speed;
	zVelocity *= speed;
	newParticle->setVelocity(Vector3(xVelocity, yVelocity, zVelocity));
	newParticle->setPosition(Vector3(0.0f, 4.0f, 6.0f));
	// Give the particle a ramp up time to make a GIF
	newParticle->setStartupTime(5.0f);
	newParticle->setColor(color);
	newParticle->setSize(size);
	newParticle->setLifeTime(3.0f);
	return newParticle;
}