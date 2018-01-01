#include "ModelMatrix.h"

ModelMatrix::ModelMatrix(void)
{
	angles[0]	 = angles[1]	= angles[2] = 0.0f;
	transData[0] = transData[1] = transData[2] = 0.0f;
	scaleData[0] = scaleData[1] = scaleData[2] = 1.0f;
}


ModelMatrix::~ModelMatrix(void)
{
}

void ModelMatrix::createModelMat( GLfloat* inAngles, GLfloat* inTransData, GLfloat* inScaleData)
{	
	for( int i = 0; i < 3; i++ )
	{
		angles[i] = inAngles[i];
		transData[i] = inTransData[i];
		scaleData[i] = inScaleData[i];
	}
	
	return;
}

void ModelMatrix::sendVars( )
{
	glUniform3fv(anglesLocation,1, angles);
	glUniform3fv(transLocation,	1, transData);
	glUniform3fv(scaleLocation,	1, scaleData);

	return;
}

void ModelMatrix::findLocations( GLuint program, char* angles, char* trans, char* scale )
{
	anglesLocation	= glGetUniformLocation(program, angles);
	transLocation	= glGetUniformLocation(program, trans);
	scaleLocation	= glGetUniformLocation(program, scale);
}