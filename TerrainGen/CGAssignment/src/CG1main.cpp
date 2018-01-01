#include "CG1main.h"

/*
CG1main::CG1main(void)
{
}

CG1main::~CG1main(void)
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

	//The player character
	player1.playerShape.makePointYLine(2, 1.0);
	player1.playerShape.initModelMatrix( voxelShader.getProgram(), "angles", "trans", "scale" );
	player1.initViewMatrix( voxelShader.getProgram(), "eye",  "target", "up" );
	player1.viewMat.eye[1] = 2.0f;
	
	//The NPC's
	for( int i = 0; i < 3; i++ )
	{
		npcs[i].makePointYLine(2, 1.0f);
		npcs[i].initModelMatrix( voxelShader.getProgram(), "angles", "trans", "scale" );
		npcCollide[i] = false;
	}

	npcs[0].translate( (float)(rand() % 20 - 10 ), 0.0f, (float)(rand() % 20 - 10 ) );
	npcs[1].translate( (float)(rand() % 20 - 10 ), 0.0f, (float)(rand() % 20 - 10 ) );
	npcs[2].translate( (float)(rand() % 20 - 10 ), 0.0f, (float)(rand() % 20 - 10 ) );

	//The floor
	theFloor.makePointPlane(50, 50);
	theFloor.initModelMatrix( voxelShader.getProgram(), "angles", "trans", "scale" );

	//The sword
	sword.makePointYLine( 10, 0.1f );
	sword.initModelMatrix( voxelShader.getProgram(), "angles", "trans", "scale" );

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
bool stop = false;

//Display function
void RenderScene(void)
{
	if( stop )
		return;
	else
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

		//Draw the Ground
		thelp.LoadBMPTexture("textures//arenaBlock.bmp");
		theFloor.drawPoints(vertexVBO, indexVBO, vertexLoc, normalLoc);

		//Draw the Character
		thelp.LoadBMPTexture("textures//characterBlock.bmp");
		player1.playerShape.drawPoints(vertexVBO, indexVBO, vertexLoc, normalLoc);

		//Draw the NPC's
		for( int i = 0; i < 3; i++ )
		{
			//collision check with sword
			if( npcCollide[i] == true )
				thelp.LoadBMPTexture("textures//characterHit.bmp");
			else
				thelp.LoadBMPTexture("textures//characterBlock.bmp");
			
			npcs[i].drawPoints(vertexVBO, indexVBO, vertexLoc, normalLoc);
		}

		//Draw the Sword
		thelp.LoadBMPTexture("textures//swordBlock.bmp");
		glUniform1f(sizeLocation, 0.1f);
		M3DVector3f vCross;
		m3dCrossProduct3(vCross, player1.viewMat.up, player1.viewMat.forward);

		sword.setTranslate( player1.viewMat.target[0] - vCross[0]*0.5f, 
							player1.viewMat.target[1] - 0.2f,
							player1.viewMat.target[2] - vCross[2]*0.5f );
	
		//Idle Animation, no buttons pressed
		if( holdAnimation == false && swingAnimation == false )
		{
			if( sword.getRotation()[0] > 60.0f )
			{
				sword.rotate( -90.0f * (float)deltaTime * 5, 0.0f, 0.0f );
			}
		}
		
		//Left click is being held down
		if( holdAnimation == true )
		{
			if( sword.getRotation()[0] > 10.0f )
			{
				sword.rotate( -50.0f * (float)deltaTime * 5, 0.0f, 0.0f );
			}	
		}
	
		//Left click is released and attack begins
		if( swingAnimation == true )
		{
			//begin collision test
			for(int i = 0; i < 3; i++ )
			{
				float x = sword.getTranslation()[0] - npcs[i].getTranslation()[0];
				float z = sword.getTranslation()[2] - npcs[i].getTranslation()[2];

				if( x > -0.5 && x < 0.5 )
					if( z > -0.5 && z < 0.5 ) 
						npcCollide[i] = true;
			}

			if( sword.getRotation()[0] < 150.0f )
			{
				sword.rotate( 140.0f * (float)deltaTime * 5, 0.0f, 0.0f );
			}
			else
				swingAnimation = false;
		}

		sword.drawPoints( vertexVBO, indexVBO, vertexLoc, normalLoc);

		// Perform the buffer swap to display back buffer
		glutSwapBuffers();
		glutPostRedisplay();

		endTime = clock();
		deltaTime = (float)(endTime - startTime) / CLOCKS_PER_SEC;
		if( player1.firstPerson || player1.thirdPerson )
			totalTime += deltaTime;

		if( npcCollide[0] == true && npcCollide[1] == true && npcCollide[2] == true )
		{
			printf("time taken: %f", totalTime );
			stop = true;
		}
	}
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

	sword.rotate( 0.0f, -deltaX, 0.0f );
	player1.rotateCameraXY( (GLfloat)m3dDegToRad(deltaY), (GLfloat)m3dDegToRad(-deltaX) );
}

void mouseClick( int button, int state, int x, int y)
{
	if( state == GLUT_DOWN && swingAnimation == false)
	{
		holdAnimation = true;
	}

	if( state == GLUT_UP )
	{
		sword.setRotate( 10.0f, sword.getRotation()[1], sword.getRotation()[2] );
		holdAnimation  = false;
		swingAnimation = true;
	}

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

	if( key_state['f'] )
	{
		player1.thirdPerson = false;
		player1.firstPerson = true;
	}

	if( key_state['t'] )
	{
		player1.thirdPerson = true;
		player1.firstPerson = false;
	}

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
*/
