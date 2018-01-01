#include "TerrainMain.h"


TerrainMain::TerrainMain(void)
{
}


TerrainMain::~TerrainMain(void)
{
}
void updateCamera();

void ChangeSize(int w, int h)
{
	if( h == 0 )
		h = 1;
	glViewport(0, 0, w, h);
	projMat.createProjection( 90.0f, float(w) / float(h), 0.1f, 1000.0f );
}

void SetupShaders()
{
	//initialize shaderProgram
	voxelShader.init();
	
	//load the source from files
	voxelShader.loadVertexShaderFile  ( "shaders\\modelViewProjection.vp" );
	voxelShader.loadGeometryShaderFile( "shaders\\PointsToCubes.gp" );
	voxelShader.loadFragmentShaderFile( "shaders\\Phong.fp" );
	
	//link and use the shader program
	voxelShader.useShaderProgram();
}

void SetupRC()
{
	// Seed random number generator
	srand(time(NULL));

	// Grey background
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f );
	
	//Enable some simple GLFeatures 
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	//Find the vertex and normal attributes in the Shader
	vertexLoc = glGetAttribLocation(voxelShader.getProgram(), "vVertex" );
	normalLoc = glGetAttribLocation(voxelShader.getProgram(), "vNormal" );

	//Generate the Vertex Buffer Object, and Index Buffer Object
	glGenBuffers(1, &vertexVBO);
	glGenBuffers(1, &indexVBO);

	//Generate the texture Object ID
	GLuint texLoc;
	glGenTextures(1, &texLoc); 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, texLoc);

	//The Player
	player1.initViewMatrix( voxelShader.getProgram(), "eye",  "target", "up" );
	player1.viewMat.eye[1] = 2.0f;

	//The Floor
	theFloor.makePointPlane(50, 50);
	theFloor.initModelMatrix( voxelShader.getProgram(), "angles", "trans", "scale" );

	//The World
	//world.makePointTerrain(NULL, NULL);
	//world.initModelMatrix( voxelShader.getProgram(), "angles", "trans", "scale" );
	for( int i = 0; i < worldLen; i++ )
		for( int j = 0; j < worldLen; j++ )
		{
			world[(i*worldLen)+j].clearShape();
			if( i == 0 && j == 0 )
			{
				//Original chunk
				world[(i*worldLen)+j].makePointTerrain( NULL, NULL); 
			}
			else if( i == 0 )
			{
				//Based on neighboring X chunk only
				world[(i*worldLen)+j].makePointTerrain( NULL, world[(i*worldLen)+(j-1)].getVerticies() );
			}
			else if( j == 0 )
			{
				//Based on neighboring Z chunk only
				world[(i*worldLen)+j].makePointTerrain( world[((i-1)*worldLen)+j].getVerticies(), NULL);  
			}
			else
			{
				//Based on both X and Z neighboring chunks
				world[(i*worldLen)+j].makePointTerrain( world[((i-1)*worldLen)+j].getVerticies(), world[(i*worldLen)+(j-1)].getVerticies()); 
			}

			world[(i*worldLen)+j].initModelMatrix(voxelShader.getProgram(), "angles", "trans", "scale" );
			world[(i*worldLen)+j].setTranslate( (i * 64.0f) + -64.0f, 0.0 , (j * 64.0f) + -64.0f );
		}

	//Helper function to make sure no spaces are in between blocks
	for( int i = 0; i < worldLen * worldLen; i++ )
		world[i].fillTerrainGaps( 65 );

	projMat.projLocation	= glGetUniformLocation(voxelShader.getProgram(), "projectionVars");
	
	GLuint lightLocation	= glGetUniformLocation(voxelShader.getProgram(), "lightPosition");
	GLuint ambLocation		= glGetUniformLocation(voxelShader.getProgram(), "ambientProduct");
	GLuint diffLocation		= glGetUniformLocation(voxelShader.getProgram(), "diffuseProduct");
	GLuint specLocation		= glGetUniformLocation(voxelShader.getProgram(), "specularProduct");
	GLuint shinLocation		= glGetUniformLocation(voxelShader.getProgram(), "shininess");

	//Send the projection data over
	projMat.createProjection( 90.0f, float(WIDTH) / float(HEIGHT), 0.1f, 1000.0f );
	projMat.sendVars();

	//Send Lighting and Material data over
	glUniform4fv(lightLocation,	1, lightPosition);
	glUniform4fv(ambLocation,	1, ambientProduct);
	glUniform4fv(diffLocation,	1, diffuseProduct);
	glUniform4fv(specLocation,	1, specularProduct);
	glUniform1f(shinLocation, shininess);
}

//clean up
void ShutdownRC()
{
	glDeleteProgram( voxelShader.getProgram() );
}

float deltaTime = 1.0f;
float totalTime = 0.0f;
clock_t startTime, endTime;

//Display function
void RenderScene(void)
{
	startTime = clock();
	updateCamera();

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glCullFace(GL_BACK);

	//Send camera information
	player1.sendCamera();

	//set the block size to 1
	GLuint sizeLocation		= glGetUniformLocation(voxelShader.getProgram(), "size");
	glUniform1f(sizeLocation, 1.0f);

	//Draw the Terrain
	thelp.LoadBMPTexture("textures//grassBlock.bmp");
	for( int i = 0; i < worldLen * worldLen; i++ )
		world[i].drawPoints(vertexVBO, indexVBO, vertexLoc, normalLoc);

	// Perform the buffer swap to display back buffer
	glutSwapBuffers();
	glutPostRedisplay();

	endTime = clock();
	deltaTime = (float)(endTime - startTime) / CLOCKS_PER_SEC;
}


void mouseMove( int x, int y )
{
	//free fly mode
	int centerX = 400;
	int centerY = 300;

	float deltaX = (float)(x - centerX) / 10.0f;
	float deltaY = (float)(y - centerY) / 10.0f;

	if( deltaX == 0 && deltaY == 0 ) return;

	glutWarpPointer( centerX, centerY );

	player1.rotateCameraXY( (GLfloat)m3dDegToRad(deltaY), (GLfloat)m3dDegToRad(-deltaX) );
}

void mouseClick( int button, int state, int x, int y)
{

}

void keyDown(unsigned char key, int x, int y)
{
    key_state[key] = true;
}
 
void keyUp(unsigned char key, int x, int y)
{
    key_state[key] = false;
}

//called everytime the frame is updated
void updateCamera(void)
{	
	float step = 0.1f;
	GLfloat turn = (GLfloat)m3dDegToRad(5.0f);

	//Escape Key
	if(key_state[27])
		exit(0);

	if( key_state['w'] ) 
		player1.moveForward( step );

    if( key_state['a'] ) 
		player1.moveLeft( step );
		
    if( key_state['s'] )
		player1.moveForward( -step );

    if( key_state['d'] ) 
		player1.moveLeft( -step );

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Demo");
    
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);

	glutSetCursor(GLUT_CURSOR_NONE);
	//glutFullScreen();

	glutIdleFunc(updateCamera);

	GLenum err = glewInit();
	if (GLEW_OK != err) 
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	SetupShaders();
	SetupRC();

	glutMainLoop();
	ShutdownRC();

	return 0;
}