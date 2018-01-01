#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GL.h>

#include <stdio.h>

class ModelMatrix
{
	public:
		GLuint anglesLocation;
		GLuint transLocation;
		GLuint scaleLocation;

		GLfloat angles[3];
		GLfloat transData[3];
		GLfloat scaleData[3];

		ModelMatrix(void);
		~ModelMatrix(void);

		void createModelMat(GLfloat* angles, GLfloat* transData, GLfloat* scaleData);
		void findLocations( GLuint program, char* angles, char* trans, char* scale );

		void sendVars();
};

