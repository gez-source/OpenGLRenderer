#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H

#include "Vector3.h"
#include <SFML/Window/Keyboard.hpp>
#include "Transform.h"

class CharacterController
{
public:

	Vector3				lightRotation		= Vector3::Zero;
	Vector3				lightDirection		= Vector3::Zero;
	Vector3				modelRotation		= Vector3::Zero;
	Vector3				cursorForward		= Vector3::Zero;
	Vector3             direction			= Vector3::Zero;
	Vector3             rotation			= Vector3::Zero;
	float				movementSpeed		= 0;
	float				sprint				= 0.7f;
	float               speed				= 0.1f;
	float               rotSpeed			= 1.5f;
	Vector3				worldPosition		= Vector3::Zero;

	Transform			cameraTransform;
	Matrix4				modelview			= Matrix4::GetIdentity();
	Matrix4				lightModelView		= Matrix4::GetIdentity();
	int					prevx = 0;
	int					prevy = 0;
	int					dx;
	int					dy;

	void OnUpdate();
	void OnFocus();

private:
	void PollInput();

};

#endif