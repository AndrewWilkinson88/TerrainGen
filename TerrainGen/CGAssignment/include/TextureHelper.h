#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GL.h> 

#include <stdio.h>

#pragma pack(1)
struct RGB { 
  GLbyte blue;
  GLbyte green;
  GLbyte red;
  GLbyte alpha;
};

struct BMPInfoHeader {
  GLuint	size;
  GLuint	width;
  GLuint	height;
  GLushort  planes;
  GLushort  bits;
  GLuint	compression;
  GLuint	imageSize;
  GLuint	xScale;
  GLuint	yScale;
  GLuint	colors;
  GLuint	importantColors;
};

struct BMPHeader {
  GLushort	type; 
  GLuint	size; 
  GLushort	unused; 
  GLushort	unused2; 
  GLuint	offset; 
}; 

struct BMPInfo {
  BMPInfoHeader		header;
  RGB				colors[1];
};
#pragma pack(8)

class TextureHelper
{
	public:
		TextureHelper(void);
		~TextureHelper(void);

		bool LoadBMPTexture( const char *fileName );
		
	private:
		GLbyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight);

};

