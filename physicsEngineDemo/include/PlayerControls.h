#pragma once
#ifndef PLAYER_CONTROLS_H
#define PLAYER_CONTROLS_H

#include "MathDataTypes.h"
#include "DataTypeRedefinition.h"

#define PLAYER_DEFAULT_SPEED 1.0f
#define PLAYER_DEFAULT_POSITION Vector3(0.0f, 4.0f, 0.0f)

namespace PhysicsDemo
{
	class PlayerController 
	{
	private:
		Vector3 position;
		real yaw;
		real pitch;
		real speed;

		bool wPressed;
		bool aPressed;
		bool sPressed;
		bool dPressed;
		
	public:
		PlayerController()
		{
			position = PLAYER_DEFAULT_POSITION;
			speed = PLAYER_DEFAULT_SPEED;
			wPressed = false;
			aPressed = false;
			sPressed = false;
			dPressed = false;
		}
		// Update player position as needed given the duration of the past frame
		void update(float deltaTime);

		// Controls
		void keyCheck(unsigned char key);
		void keyUpCheck(unsigned char key);

		// Getters and setters
		real getSpeed();
		void setSpeed(real newSpeed);

		Vector3 getPosition();
		void setPosition(Vector3 newPosition);

		real getYaw();
		void setYaw(real newYaw);

		real getPitch();
		void setPitch(real newPitch);
	};
}

#endif //PLAYER_CONTROLS_H