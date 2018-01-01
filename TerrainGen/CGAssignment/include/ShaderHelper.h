#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GL.h>

#include <stdio.h>

class ShaderHelper
{
	private:
		int loadShader(GLuint shader, const GLchar** input );
		int loadShaderFromFile(GLuint shader, const char* filename );

		GLuint vertexShader;
		GLuint geometryShader;
		GLuint fragmentShader;
		GLuint program;

	public:
		
		ShaderHelper(void);
		~ShaderHelper(void);

		void init();
		
		//Loading shaders from file 
		int loadVertexShaderFile(const char* fileName);
		int loadFragmentShaderFile(const char* fileName);
		int loadGeometryShaderFile(const char* fileName);

		void bindAttribute(GLuint location, GLchar* name);
		int useShaderProgram(void);
		GLuint getProgram(void) { return program; }


};

