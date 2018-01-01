#include "SimpleShape.h"


SimpleShape::SimpleShape(void)
{
	pointArray = NULL;
	normalArray = NULL;
	elemArray = NULL;
}


SimpleShape::~SimpleShape(void)
{
}

/**
 * Clears out a current shape
 */
void SimpleShape::clearShape ()
{
    if (pointArray) {
        delete pointArray;
		delete normalArray;
        delete elemArray;
        pointArray = 0;
		normalArray = 0;
        elemArray = 0;
    }
    points.clear();
	normals.clear();
}

/**
 * gets the vertex points for the current shape
 */
float* SimpleShape::getVerticies ()
{
    // delete the old point array of we have one
    if (pointArray) {
        free(pointArray);
    }
    
    // create and fill a new point array
    pointArray = new float[points.size()];
    for (GLuint i = 0; i < points.size(); i++) {
        pointArray[i] = points[i];
    }
        
    return pointArray;
}

/**
 * gets the normals for the current shape
 */
float* SimpleShape::getNormals ()
{
    // delete the old point array of we have one
    if (normalArray) {
        free(normalArray);
    }
    
    // create and fill a new point array
    normalArray = new float[normals.size()];
    for (GLuint i = 0; i < normals.size(); i++) {
        normalArray[i] = normals[i];
    }
        
    return normalArray;
}

/**
 * gets the  array of elements for the  current shape
 */
GLushort* SimpleShape::getElements ()
{
    // delete the old point array of we have one
    if (elemArray) {
        free(elemArray);
    }
    
    // create and fill a new point array
    elemArray = new GLushort[points.size()];
    for (GLuint i = 0; i < points.size(); i++) {
        elemArray[i] = i;
    }
    
    return elemArray;
}

/**
 * returns number and size of data in current shape
 */
int SimpleShape::numVerticies()	{ return points.size(); }
int SimpleShape::numNormals()	{ return normals.size(); }

int SimpleShape::dataSize()		{ return sizeof(float) * numVerticies(); }
int SimpleShape::normDataSize() { return sizeof(float) * numNormals(); }
int SimpleShape::elemDataSize() { return sizeof(GLushort) * numVerticies() / 4; }

/**
*	Fills the array buffers and calls draw elements, with points
**/
void SimpleShape::drawPoints(GLuint vertexVBO, GLuint indexVBO, GLuint vertexLoc, GLuint normalLoc)
{
	modMat.sendVars();

	glBindBuffer( GL_ARRAY_BUFFER, vertexVBO );
	glBufferData( GL_ARRAY_BUFFER, dataSize() + normDataSize(), (void*)0 , GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, (GLintptr)BUFFER_OFFSET( 0 ), dataSize(), getVerticies() );
	glBufferSubData( GL_ARRAY_BUFFER, (GLintptr)BUFFER_OFFSET( dataSize() ), normDataSize(), getNormals() );

	glEnableVertexAttribArray( vertexLoc );
	glVertexAttribPointer( vertexLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( 0 ) ); 
	glEnableVertexAttribArray( normalLoc );
	glVertexAttribPointer( normalLoc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( dataSize() ) ); 

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, elemDataSize(), getElements() , GL_STATIC_DRAW );

	//Draw elements to screen
	glDrawElements( GL_POINTS, elemDataSize() / sizeof(GLushort), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0) );	
}

void SimpleShape::makePointPlane( int rows, int columns )
{
	float sizeX = (0.5f * rows);
	float sizeZ = (0.5f * columns);

	for( GLfloat x = -sizeX; x <= sizeX; x += 1.0f)
		for( GLfloat z = -sizeZ; z <= sizeZ; z += 1.0f)
		{
			points.push_back(x);
			points.push_back(-0.55f);
			points.push_back(z);
			points.push_back(1.0);

			normals.push_back (0.0f);
			normals.push_back (1.0f);
			normals.push_back (0.0f);
			normals.push_back (0.0f);
		}
}

void SimpleShape::makePointYLine(int numBlocks, float bSize)
{
	float size = (float)numBlocks;

	for( GLfloat y = bSize/2; y < size * bSize; y += bSize)
	{
		points.push_back(0.0f);
		points.push_back(y);
		points.push_back(0.0f);
		points.push_back(1.0);

		normals.push_back (0.0f);
		normals.push_back (1.0f);
		normals.push_back (0.0f);
		normals.push_back (0.0f);
	}
}

void SimpleShape::translate(float x, float y, float z)
{
	modMat.transData[0] += x;
	modMat.transData[1] += y;
	modMat.transData[2] += z;
}

void SimpleShape::setTranslate(float x, float y, float z)
{
	modMat.transData[0] = x;
	modMat.transData[1] = y;
	modMat.transData[2] = z;
}

GLfloat* SimpleShape::getTranslation() 
{
	return modMat.transData;
}

void SimpleShape::rotate(float x, float y, float z)
{
	modMat.angles[0] += x;
	modMat.angles[1] += y;
	modMat.angles[2] += z;
}

void SimpleShape::setRotate(float x, float y, float z)
{
	modMat.angles[0] = x;
	modMat.angles[1] = y;
	modMat.angles[2] = z;
}

GLfloat* SimpleShape::getRotation() 
{
	return modMat.angles;
}

void SimpleShape::initModelMatrix(GLuint program, char* angles, char* trans, char* scale)
{
	modMat.findLocations( program, angles, trans, scale );
}