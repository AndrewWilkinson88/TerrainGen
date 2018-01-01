#include "ViewMatrix.h"

ViewMatrix::ViewMatrix(void)
{
	eye[0] = 0.0f;
	eye[1] = 0.0f;
	eye[2] = 0.0f;

	target[0] = 0.0f;
	target[1] = 0.0f;
	target[2] = 0.0f;

	forward[0] = 0.0f;
	forward[1] = 0.0f;
	forward[2] = 1.0f;

	up[0] = 0.0f;
	up[1] = 1.0f;
	up[2] = 0.0f;
}

ViewMatrix::~ViewMatrix(void)
{
}

void ViewMatrix::rotateWorld( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
	M3DMatrix44f rotMat;

	// Create the Rotation matrix
	m3dRotationMatrix44(rotMat, angle, x, y, z);

	M3DVector3f newVect;
			
	// Transform the up axis (inlined 3x3 rotation)
	newVect[0] = rotMat[0] * up[0] + rotMat[4] * up[1] + rotMat[8] *  up[2];	
	newVect[1] = rotMat[1] * up[0] + rotMat[5] * up[1] + rotMat[9] *  up[2];	
	newVect[2] = rotMat[2] * up[0] + rotMat[6] * up[1] + rotMat[10] * up[2];	
	m3dCopyVector3(up, newVect);

	// Transform the forward axis
	newVect[0] = rotMat[0] * forward[0] + rotMat[4] * forward[1] + rotMat[8] *  forward[2];	
	newVect[1] = rotMat[1] * forward[0] + rotMat[5] * forward[1] + rotMat[9] *  forward[2];	
	newVect[2] = rotMat[2] * forward[0] + rotMat[6] * forward[1] + rotMat[10] * forward[2];	
	m3dCopyVector3(forward, newVect);
}

void ViewMatrix::rotateLocalX( GLfloat angle )
{
	M3DMatrix33f rotMat;
	M3DVector3f  localX;
	M3DVector3f  rotVec;

	// Get the local X axis
	m3dCrossProduct3(localX, up, forward);

	// Make a Rotation Matrix
	m3dRotationMatrix33(rotMat, angle, localX[0], localX[1], localX[2]);

	// Rotate Y, and Z
	m3dRotateVector(rotVec, up, rotMat);
	m3dCopyVector3(up, rotVec);

	m3dRotateVector(rotVec, forward, rotMat);
	m3dCopyVector3(forward, rotVec);
}

void ViewMatrix::findLocations(GLuint program, char* eye, char* target, char* up )
{
	eyeLocation = glGetUniformLocation(program, eye);
	targetLocation = glGetUniformLocation(program, target);
	upLocation = glGetUniformLocation(program, up);
}

void ViewMatrix::sendFirstPerson(void)
{
	m3dNormalizeVector3(up);
	m3dNormalizeVector3(forward);

	target[0] = eye[0] + forward[0];
	target[1] = eye[1] + forward[1];
	target[2] = eye[2] + forward[2];

	glUniform3fv(eyeLocation,	1, eye);
	glUniform3fv(targetLocation,1, target);
	glUniform3fv(upLocation,	1, up);
}


void ViewMatrix::sendThirdPerson(void)
{
	m3dNormalizeVector3(up);
	m3dNormalizeVector3(forward);

	target[0] = eye[0];
	target[1] = eye[1];
	target[2] = eye[2];

	float tempEye[3];
	tempEye[0] = eye[0] - forward[0] * 5;
	tempEye[1] = eye[1] - forward[1] * 5;
	tempEye[2] = eye[2] - forward[2] * 5;

	glUniform3fv(eyeLocation,	1, tempEye);
	glUniform3fv(targetLocation,1, target);
	glUniform3fv(upLocation,	1, up);
}

void ViewMatrix::sendVars(void)
{	
	m3dNormalizeVector3(up);
	m3dNormalizeVector3(forward);

	target[0] = eye[0];
	target[1] = eye[1];
	target[2] = eye[2];

	eye[0] -= forward[0] * 5;
	eye[1] -= forward[1] * 5;
	eye[2] -= forward[2] * 5;

	/*
	target[0] = eye[0] + forward[0];
	target[1] = eye[1] + forward[1];
	target[2] = eye[2] + forward[2];
	*/

	glUniform3fv(eyeLocation,	1, eye);
	glUniform3fv(targetLocation,1, target);
	glUniform3fv(upLocation,	1, up);

	eye[0] += forward[0] * 5;
	eye[1] += forward[1] * 5;
	eye[2] += forward[2] * 5;
}

void ViewMatrix::moveForward(GLfloat amount)
{
	eye[0] += forward[0] * amount;
	eye[1] += forward[1] * amount;
	eye[2] += forward[2] * amount;
}

void ViewMatrix::moveLeft(GLfloat amount)
{
	M3DVector3f vCross;
	m3dCrossProduct3(vCross, up, forward);

	eye[0] += vCross[0] * amount;
	eye[1] += vCross[1] * amount;
	eye[2] += vCross[2] * amount;
}

void ViewMatrix::moveDown(GLfloat amount)
{
	return;
}

