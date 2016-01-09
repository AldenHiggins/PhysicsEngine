#ifndef PARTICLE_H
#define PARTICLE_H

#ifdef PHYSICSDLL_EXPORTS
#define PHYSICSDLL_API __declspec(dllexport) 
#else
#define PHYSICSDLL_API __declspec(dllimport) 
#endif


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
		PHYSICSDLL_API virtual void integrate(real timeStep);
		// Getters and setters for particle data
		PHYSICSDLL_API Vector3 getPosition();
		PHYSICSDLL_API void setPosition(Vector3 newPosition);
		PHYSICSDLL_API Vector3 getVelocity();
		PHYSICSDLL_API void setVelocity(Vector3 newVelocity);
		PHYSICSDLL_API Vector3 getAcceleration();
		PHYSICSDLL_API void setAcceleration(Vector3 newAcceleration);
		PHYSICSDLL_API real getLifeTime();
		PHYSICSDLL_API void setLifeTime(real newLifeTime);
		PHYSICSDLL_API bool getIsDead();
		PHYSICSDLL_API void setStartupTime(real startupTime);
	};
}


#endif //PARTICLE_H