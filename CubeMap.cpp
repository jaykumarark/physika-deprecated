#include "CubeMap.h"


CubeMap::CubeMap(vector<string> filename, GLint wrap, GLfloat minFilter, GLfloat magFilter)
{
	m_wrap = wrap;
	m_minFilter = minFilter;
	m_magFilter = magFilter;
	init();
}

CubeMap::~CubeMap(void)
{
	glDeleteTextures(1, &mTextureId);
}

void CubeMap::init()
{
	glGenTextures  (1, &mTextureId);	
	glBindTexture  (GL_TEXTURE_CUBE_MAP, mTextureId);

	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 
	};

	glTexImage2D   (targets[0], 0, GL_RGB, mImage->width(), mImage->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, mImage->data());

	//Texture Settings
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_magFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrap);
	glBindTexture  (GL_TEXTURE_CUBE_MAP, 0);	//unbind
}

void CubeMap::activate(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
}

void CubeMap::deactivate()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


