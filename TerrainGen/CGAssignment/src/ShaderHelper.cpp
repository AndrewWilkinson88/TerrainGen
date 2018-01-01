#include "ShaderHelper.h"


ShaderHelper::ShaderHelper(void)
{
}

ShaderHelper::~ShaderHelper(void)
{
}

void ShaderHelper::init()
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();
}

int ShaderHelper::loadShader( GLuint shader, const GLchar** input )
{
	glShaderSource( shader, 1, input, NULL );
	glCompileShader( shader );

	int result = 0;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &result );

	if( result == GL_FALSE)
	{
		char infoLog[1000];
		glGetShaderInfoLog( shader, 1000, NULL, infoLog );
		printf( "The shader has compilation errors!\n" );
		printf( "%s", infoLog );
		return 1;
	}
	
	glAttachShader(program, shader);
	return 0;
}

int ShaderHelper::loadShaderFromFile(GLuint shader, const char* filename )
{
	FILE *file;
	int fileLength = 0;
    fopen_s(&file, filename, "r");

	while ( fgetc(file) != EOF)
		fileLength++;

	rewind(file);
	char* shaderBuf = new char[fileLength+1];

	fread( shaderBuf, 1, fileLength, file );
	shaderBuf[fileLength] = '\0';
	fclose(file);

	GLchar *stringPtr[1];
	stringPtr[0] = (GLchar *)shaderBuf;
	return loadShader( shader, (const GLchar**)stringPtr );
}

//Load the Individual shaders
int ShaderHelper::loadVertexShaderFile(const char* fileName)
{
	return loadShaderFromFile(vertexShader, fileName );
}

int ShaderHelper::loadGeometryShaderFile(const char* fileName)
{
	return loadShaderFromFile(geometryShader, fileName );
}

int ShaderHelper::loadFragmentShaderFile(const char* fileName)
{
	return loadShaderFromFile(fragmentShader, fileName );
}

//Add an attribute to the program
void ShaderHelper::bindAttribute( GLuint location, GLchar* name)
{
	glBindAttribLocation(program, location, name);
}

//Link and use the shader program
int ShaderHelper::useShaderProgram(void)
{
	glLinkProgram(program);

	//once linked we no longer need the individual shaders
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	int result = 0;
	glGetProgramiv( program, GL_LINK_STATUS, &result);
	
	if( result == GL_FALSE )
	{
		char infoLog[1000];
		glGetProgramInfoLog(program, 1000, NULL, infoLog);
		printf( "The shader program has linker errors!\n" );
		printf( "%s", infoLog );
		return 1;
	}

	glUseProgram(program);
	return 0;
}