#include "PlayerCharacter.h"

PlayerCharacter::PlayerCharacter(void)
{
	currRotX = 0.0;
	firstPerson = true;
	thirdPerson = false;
}

PlayerCharacter::~PlayerCharacter(void)
{
}

void PlayerCharacter::sendCamera(void)
{
	if( firstPerson )
		viewMat.sendFirstPerson();
	if (thirdPerson )
		viewMat.sendThirdPerson();
}

//Limits rotation to -90 and 90 on both axiis
void PlayerCharacter::rotateCameraXY(GLfloat rotX, GLfloat rotY)
{
	//First rotate view on the Y rotY radians
	viewMat.rotateWorld( rotY, 0.0f, 1.0f, 0.0f);

	//Ended up having to use 60 and -90 for some reason
	if( !( (currRotX + rotX) > (3.14159 / 3.0) || (currRotX + rotX) < -(3.14159 / 2.0) ) )
	{
		currRotX += rotX;

		//Given that new orientation rotate on the X rotX radians
		viewMat.rotateLocalX( rotX );
	}
}

void PlayerCharacter::moveForward(GLfloat amount)
{
	viewMat.moveForward(amount);
	playerShape.setTranslate(viewMat.eye[0], viewMat.eye[1]-2.0f, viewMat.eye[2]);
}

void PlayerCharacter::moveLeft(GLfloat amount)
{
	viewMat.moveLeft(amount);
	playerShape.setTranslate(viewMat.eye[0], viewMat.eye[1]-2.0f, viewMat.eye[2]);
}

void PlayerCharacter::moveDown(GLfloat amount)
{
	viewMat.moveDown(amount);
	playerShape.setTranslate(viewMat.eye[0], viewMat.eye[1], viewMat.eye[2]);
}

void PlayerCharacter::initViewMatrix(GLuint program, char* eye, char* target, char* up)
{
	viewMat.findLocations( program, eye, target, up );
}