/*
#pragma once
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define WIDTH 800
#define HEIGHT 600

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GL.h>

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "PlayerCharacter.h"
#include "ProjectionMatrix.h"

#include "ShaderHelper.h"
#include "TextureHelper.h"
#include "SimpleShape.h"

ShaderHelper voxelShader;
ShaderHelper objectShader;

PlayerCharacter	player1;
ProjectionMatrix projMat;
TextureHelper thelp;

GLuint vertexLoc;
GLuint normalLoc;
GLuint projLocation;

GLuint	vertexVBO;
GLuint	indexVBO;

//visible shapes
SimpleShape theFloor;
SimpleShape npcs[3];
SimpleShape sword;
bool npcCollide[3];
bool holdAnimation	= false;
bool swingAnimation	= false;

//my vertices
struct Vertex
{
	float x, y, z;
};

bool key_state[256] = { false };

//define material properties
float lightRad = 32.0 * 3.0;
float ang = 0.0f;
GLfloat lightPosition[] = {0.0f, 10.0f, 0.0f, 1.0f};
GLfloat ambientProduct[] = {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat diffuseProduct[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat specularProduct[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat shininess = 300.0f;

class CG1main
{
	public:
		CG1main(void);
		~CG1main(void);
};

*/
