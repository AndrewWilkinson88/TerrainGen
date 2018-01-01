#pragma once
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define SQRT3 1.73205081

#include <GL/glew.h>
#include <GL/gl.h>
#include <time.h>

#include <vector>
#include "ModelMatrix.h"

using namespace std;

struct Point
{
	float x, y, z;
};

struct Face
{
	Point a;
	Point b;
	Point c;
	Point d;
};

class SimpleShape
{
	public:

		SimpleShape(void);
		~SimpleShape(void);

		void clearShape();
		
		void makePointPlane( int rows, int columns );
		void makePointYLine( int numBlocks, float bSize );

		float *getVerticies ();
		void drawPoints(GLuint vertexVBO, GLuint indexVBO, GLuint vertexLoc, GLuint normalLoc);
		
		void translate( float x, float y, float z );
		void setTranslate( float x, float y, float z );
		GLfloat* getTranslation( );

		void rotate( float x, float y, float z );
		void setRotate( float x, float y, float z );
		GLfloat* getRotation( );

		void initModelMatrix(GLuint program, char* angles, char* trans, char* scale);
		
	protected:
		ModelMatrix	modMat;
		
		vector<float> points;
		vector<float> normals;

		float *pointArray;
		float *normalArray;
		GLushort *elemArray;

 		GLushort *getElements ();
		float *getNormals ();
		
		int numVerticies();
		int numNormals();		
		int dataSize();
		int normDataSize();
		int elemDataSize();
};