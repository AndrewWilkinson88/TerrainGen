#include "ProjectionMatrix.h"


ProjectionMatrix::ProjectionMatrix(void)
{
}


ProjectionMatrix::~ProjectionMatrix(void)
{
}

void ProjectionMatrix::createProjection( GLfloat inFov, GLfloat inAspect, GLfloat inNearZ, GLfloat inFarZ)
{
	fov = inFov;
	aspect = inAspect;
	nearZ = inNearZ;
	farZ = inFarZ;
	
	return;
}

void ProjectionMatrix::sendVars( )
{
	GLfloat vars[] = {fov, aspect, nearZ, farZ};
	glUniform4fv(projLocation,	1, vars);
	
	return;
}
