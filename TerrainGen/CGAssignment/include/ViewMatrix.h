#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GL.h>

#include <math.h>
#include <stdio.h>

#include "math3d.h"

class ViewMatrix
{
	public:
	
		GLfloat eye[3];
		GLfloat forward[3];
		GLfloat up [3];

		GLfloat target[3];

		GLuint eyeLocation;
		GLuint targetLocation;
		GLuint upLocation;
		
		ViewMatrix(void);
		~ViewMatrix(void);

		void rotateWorld( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
		void rotateLocalX( GLfloat angle );

		void findLocations(GLuint program, char* eye, char* target, char* up );
		
		void sendFirstPerson(void);
		void sendThirdPerson(void);

		void sendVars(void);
		
		
		void moveForward(GLfloat amount);
		void moveLeft(GLfloat amount);
		void moveDown(GLfloat amount);
};

