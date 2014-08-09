#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include <GL\glew.h>
#include "image.h"

using namespace std;

class Texture
{
public:
	Texture(GLenum TextureTarget, string filename, GLint wrap, GLfloat minFilter, GLfloat magFilter);
   ~Texture(void);
   /*
	textureUnit e.g., GL_TEXTURE0
	unit e.g. 0	
   */
	void activate(GLenum textureUnit);
	void deactivate();

private:
	void init();	
	GLenum mTarget;
	GLuint mTextureId;
	Image* mImage;

	//texture params
	GLint m_wrap; 
	GLfloat m_magFilter; 
	GLfloat m_minFilter;

};


#endif