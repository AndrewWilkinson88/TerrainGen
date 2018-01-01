#pragma once
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GL.h>

#include <stdio.h>

void ChangeSize(int w, int h);
void RenderScene(void);
void SetupRenderContext();

void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void UpdateCamera(void);