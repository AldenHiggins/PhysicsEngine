#ifndef RENDER_PARTICLES_H
#define RENDER_PARTICLES_H

#include "MathDataTypes.h"

// Position
// Size
// Velocity
// Color
// Shape?

namespace PhysicsDemo
{
	class Particle
	{
	protected:
		PhysicsEngine::Vector3 position;
		PhysicsEngine::Vector3 velocity;
		PhysicsEngine::Vector3 acceleration;
		PhysicsEngine::real size;
		PhysicsEngine::Vector3 color;
		PhysicsEngine::real lifeTime;
		// The amount of time the particle waits to be displayed
		PhysicsEngine::real startupTime;
		bool isDead = false;
		PhysicsEngine::real timeAliveSoFar;
	public:
		Particle()
		{
			timeAliveSoFar = 0;
		}
		// Integrate the particle forward in time by the timestep
		virtual void integrate(PhysicsEngine::real timeStep);
		// Display this particle
		virtual void display();
		// Function called when this particle dies
		virtual void onDeath();
		// Getters and setters for particle data
		PhysicsEngine::Vector3 getPosition();
		void setPosition(PhysicsEngine::Vector3 newPosition);
		PhysicsEngine::Vector3 getVelocity();
		void setVelocity(PhysicsEngine::Vector3 newVelocity);
		PhysicsEngine::Vector3 getAcceleration();
		void setAcceleration(PhysicsEngine::Vector3 newAcceleration);
		PhysicsEngine::real getSize();
		void setSize(PhysicsEngine::real newSize);
		PhysicsEngine::Vector3 getColor();
		void setColor(PhysicsEngine::Vector3 newColor);
		PhysicsEngine::real getLifeTime();
		void setLifeTime(PhysicsEngine::real newLifeTime);
		bool getIsDead();
		void setStartupTime(PhysicsEngine::real startupTime);
	};

	class CircleParticle : public Particle
	{
	public:
		// Display this particle
		virtual void display();
	};

	class FireworkParticle : public CircleParticle
	{
	private:
		Particle *deathParticles[5];
		int lives;
	public:
		// Create a new firework
		FireworkParticle(int livesInput)
		{
			lives = livesInput;
		}

		// Display this particle
		virtual void display();
		// Call this when the particle dies
		virtual void onDeath();
		// Firework integrate that integrates all of its children
		virtual void integrate(PhysicsEngine::real timeStep);
	};

	struct CreateParticle
	{
		// Generate a new firework
		static Particle *createFireWorkParticle(PhysicsEngine::real speed, PhysicsEngine::real size, PhysicsEngine::Vector3 position, PhysicsEngine::Vector3 color, bool originalFirework, int lives);
		// Generate a new particle
		static Particle* CreateParticle::createParticle(PhysicsEngine::real speed, PhysicsEngine::real size, PhysicsEngine::Vector3 color);
		// Generate a new circular particle
		static Particle* createCircularParticle(PhysicsEngine::real speed, PhysicsEngine::real size, PhysicsEngine::Vector3 color);
	private:
		// These are private to stop instances being created: use get().
		CreateParticle() {}
		CreateParticle(const CreateParticle &) {}
		CreateParticle& operator=(const CreateParticle &);
	};

	




}


#endif //RENDER_PARTICLES_H