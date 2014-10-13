#pragma once

#ifndef CUBEMAP_H
#define CUBEMAP_H
#include <iostream>
#include <GL\glew.h>
#include <vector>
#include "image.h"

using namespace std;

class CubeMap
{
public:
	CubeMap(vector<string> filename, GLint wrap, GLfloat minFilter, GLfloat magFilter);
   ~CubeMap(void);
   /*
	textureUnit e.g., GL_TEXTURE0
	unit e.g. 0	
   */
	void activate(GLenum textureUnit);
	void deactivate();

private:
	void init();	
	GLuint mTextureId;

	//texture params
	GLint m_wrap; 
	GLfloat m_magFilter; 
	GLfloat m_minFilter;
	Image* mImage;
	vector<string> mFiles;

};


#endif