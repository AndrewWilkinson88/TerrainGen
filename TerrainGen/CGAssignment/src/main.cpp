#include "main.h"

//Managers
GLFrustum viewFrustum;
GLGeometryTransform transformPipeline;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;

ShaderHelper myShader;
GLint	locMVP;				// The location of the ModelViewProjection matrix uniform
GLint   locColor;			// The location of the color value uniform

GLuint	vertexVBO;
GLuint	indexVBO;

//Camera
GLFrame			cameraFrame;
bool key_state[256] = { false };

//setup the viewport AND the projection matrix
void ChangeSize(int w, int h)
{
	//prevent divide by zero
	if( h == 0 )
		h = 1;

	glViewport(0, 0, w, h);
	viewFrustum.SetPerspective( 35.0f, float(w)/float(h), 1.0f, 100.0f);
	
	projectionMatrix.LoadMatrix( viewFrustum.GetProjectionMatrix() );
	transformPipeline.SetMatrixStacks( modelViewMatrix, projectionMatrix );
}

//display vertex data
void RenderScene(void)
{
	UpdateCamera();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	modelViewMatrix.PushMatrix();

		//Put things into perspective of Camera
		M3DMatrix44f mCamera;
		cameraFrame.GetCameraMatrix(mCamera);
		modelViewMatrix.PushMatrix(mCamera);

			//Set Modelview projection matrix
			glUniformMatrix4fv(locMVP, 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix());

		modelViewMatrix.PopMatrix();
	modelViewMatrix.PopMatrix();
	
	glDrawElements(GL_POINTS, 6400, GL_FLOAT, BUFFER_OFFSET(0));
	
	glutSwapBuffers();
	glutPostRedisplay();
}

//Load the Vertex, Geometry, and Fragment Shaders
void SetupShader()
{
	myShader.init();

	//Setup Vertex Shader
	FILE *myFile;
	int fileLength = 0;

    myFile = fopen("FlatShader.vp", "r");
    
	while (fgetc(myFile) != EOF)
		fileLength++;

	rewind(myFile);
	char* string1 = new char[fileLength];

	fread( string1, 1, fileLength, myFile );
	string1[fileLength] = '\0';
	fclose(myFile);

	GLchar *stringPtr[1];
	stringPtr[0] = (GLchar *)string1;
	myShader.loadVertexShader( (const GLchar**)stringPtr );

	//Setup Geometry Shader
	fileLength = 0;
	myFile = fopen( "FlatShader.gp", "r" );
	
	while (fgetc(myFile) != EOF)
		fileLength++;

	rewind(myFile);
	char* string2 = new char[fileLength];

	fread( string2, 1, fileLength, myFile );
	string2[fileLength] = '\0';
	fclose(myFile);

	stringPtr[0] = (GLchar *)string2;
	myShader.loadGeometryShader( (const GLchar**)stringPtr );
	
	//setup Fragment Shader
	fileLength = 0;
	myFile = fopen( "FlatShader.fp", "r" );
	
	while (fgetc(myFile) != EOF)
		fileLength++;

	rewind(myFile);
	char* string3 = new char[fileLength];

	fread( string3, 1, fileLength, myFile );
	string3[fileLength] = '\0';
	fclose(myFile);

	stringPtr[0] = (GLchar *)string3;
	myShader.loadFragmentShader( (const GLchar**)stringPtr );
	
	//link and use the shader program
	myShader.useShaderProgram();
	return;
}

//Load data to the card, and other intial data
void SetupRenderContext()
{	
	//initialize the shader
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );	

	//Generate Vertices
	struct MyVertex
	{
		float x, y, z;
	};
	
	MyVertex *pvertex = new MyVertex[6400];
	
	int i = 0;
	for( GLfloat x = -20.0f; x <= 20.0f; x += 0.5f)
		for( GLfloat z = -20.0f; z <= 20.0f; z += 0.5f)
		{
			pvertex[i].x = x;
			pvertex[i].y = -0.55f;
			pvertex[i].z = z;
			i++;
		}

	//Generate the Vertex Buffer Object
	glGenBuffers(1, &vertexVBO);
	glBindBuffer( GL_ARRAY_BUFFER, vertexVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(MyVertex) * 6400, &pvertex[0].x, GL_STATIC_DRAW);

	//Generate corresponding indices
	GLshort pindices[6400];
	for( i = 0; i < 6400; i++ )
		pindices[i] = i;

	//Generate the Index Vertex Buffer Object
	glGenBuffers(1, &indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * 6400, pindices, GL_STATIC_DRAW);

	//find variable locations
	GLuint vertexLoc = glGetAttribLocation(myShader.getProgram(), "vVertex" );
	locMVP = glGetUniformLocation(myShader.getProgram(), "mvpMatrix");
	locColor = glGetUniformLocation(myShader.getProgram(), "vColorValue");

	//Set Floot color
	static GLfloat vFloorColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	glUniform4fv(locColor, 1, vFloorColor);

	//Define pointer to vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer( vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) ); 
	
	//Redbind to Indicies
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO );	
}

void ShutdownRenderContext()
{
	glDeleteBuffers(1, &vertexVBO );
	glDeleteBuffers(1, &indexVBO );
}

void keyDown(unsigned char key, int x, int y)
{
    key_state[key] = true;
}
 
void keyUp(unsigned char key, int x, int y)
{
    key_state[key] = false;
}

void mouseMove(int x, int y)
{	
	//free fly mode
	int centerX = 400;
	int centerY = 300;

	int deltaX = (x - centerX) / 10.0;
	int deltaY = (y - centerY) / 10.0;

	if( deltaX == 0 && deltaY == 0 ) return;

	glutWarpPointer( centerX, centerY );

	cameraFrame.RotateWorld( m3dDegToRad(-deltaX), 0.0f, 1.0f, 0.0f);
	cameraFrame.RotateWorld( m3dDegToRad(deltaY), 0.0f, 1.0f, 0.0f);

	cameraFrame.RotateLocalX( m3dDegToRad(deltaY) );
}

void UpdateCamera(void)
{
	float linear = 0.1f;
	float angular = float(m3dDegToRad(5.0f));
	
	//Escape Key
	if(key_state[27] == true)
		exit(0);

	if(key_state['w'] == true) 
        cameraFrame.MoveForward(linear);
    
    if(key_state['a'] == true) 
		cameraFrame.MoveRight( linear );
		
    if(key_state['s'] == true)
        cameraFrame.MoveForward(-linear);
	
    if(key_state['d'] == true) 
        cameraFrame.MoveRight( -linear );
}


//The main entry point
int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);
	
	//Glut initial functions
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL SphereWorld");

	//Glut Binding functions
	glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);

	glutSetCursor(GLUT_CURSOR_NONE);
	//glutFullScreen();
	glutIdleFunc(UpdateCamera);

	//Glew init
	GLenum err = glewInit();
	if (GLEW_OK != err) 
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}
	
	//Begin main loop
	SetupShader();
	SetupRenderContext();
	glutMainLoop();

	ShutdownRenderContext();
	return 0;
}