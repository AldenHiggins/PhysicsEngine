#ifndef PHYSICS_ENGINE_CONTROLS_H
#define PHYSICS_ENGINE_CONTROLS_H

#include "Particle.h"
#include "RigidBody.h"
#include "RenderableObjects.h"
#include <vector>

namespace PhysicsEngine
{
	void keyCheck(unsigned char key, std::vector<Particle *> *particles, std::vector<RectangleObject *> *rigidBodies, float theta, float phi);	
}

#endif // PHYSICS_ENGINE_CONTROLS_H