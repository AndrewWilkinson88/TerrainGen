#pragma once
#include "ShaderHelper.h"
#include "ViewMatrix.h"
#include "simpleShape.h"

class PlayerCharacter
{
	public:
		PlayerCharacter(void);
		~PlayerCharacter(void);

		void sendCamera(void);
		void rotateCameraXY( GLfloat rotX, GLfloat rotY );
		void moveForward(GLfloat amount);
		void moveLeft(GLfloat amount);
		void moveDown(GLfloat amount);

		void initViewMatrix(GLuint program, char* eye, char* target, char* up);

		SimpleShape playerShape;
		ViewMatrix viewMat;
		GLfloat currRotX;
		bool firstPerson;
		bool thirdPerson;

};

