#ifndef PARTICLE_H
#define PARTICLE_H

#include "MathDataTypes.h"

namespace PhysicsEngine
{
	class Particle
	{
	protected:
		Vector3 position;
		Vector3 velocity;
		Vector3 acceleration;
		real lifeTime;
		// The amount of time the particle waits to be displayed
		real startupTime;
		bool isDead = false;
		real timeAliveSoFar;
	public:
		Particle()
		{
			timeAliveSoFar = 0;
		}
		// Integrate the particle forward in time by the timestep
		virtual void integrate(real timeStep);
		// Getters and setters for particle data
		Vector3 getPosition();
		void setPosition(Vector3 newPosition);
		Vector3 getVelocity();
		void setVelocity(Vector3 newVelocity);
		Vector3 getAcceleration();
		void setAcceleration(Vector3 newAcceleration);
		real getSize();
		void setSize(real newSize);
		real getLifeTime();
		void setLifeTime(real newLifeTime);
		bool getIsDead();
		void setStartupTime(real startupTime);
	};
}


#endif //PARTICLE_H