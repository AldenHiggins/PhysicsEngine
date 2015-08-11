#ifndef PHYSICS_ENGINE_CONTROLS_H
#define PHYSICS_ENGINE_CONTROLS_H

#include "Particle.h"
#include "RigidBody.h"
#include <vector>

namespace PhysicsEngine
{
	void keyCheck(unsigned char key, std::vector<Particle *> *particles, std::vector<RigidBody *> *rigidBodies);	
}

#endif // PHYSICS_ENGINE_CONTROLS_H