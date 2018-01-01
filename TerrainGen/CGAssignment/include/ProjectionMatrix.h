#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GL.h>

#include <stdio.h>

class ProjectionMatrix
{

public:
	GLuint projLocation;

	ProjectionMatrix(void);
	~ProjectionMatrix(void);

	void createProjection(GLfloat fov, GLfloat aspect, GLfloat nearZ, GLfloat farZ);
	void sendVars();

private:
	GLfloat fov;
	GLfloat aspect;
	GLfloat nearZ;
	GLfloat farZ;

};

