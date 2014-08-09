#include "texture.h"


Texture::Texture(GLenum TextureTarget, string filename, GLint wrap, GLfloat minFilter, GLfloat magFilter)
{
	mTarget = TextureTarget;
	m_wrap = wrap;
	m_minFilter = minFilter;
	m_magFilter = magFilter;
	mImage = new Image(filename);
	init();
}

Texture::~Texture(void)
{
	glDeleteTextures(1, &mTextureId);
	delete mImage;	
}

void Texture::init()
{
	glGenTextures  (1, &mTextureId);	
	glBindTexture  (mTarget, mTextureId);
	glTexImage2D   (mTarget, 0, GL_RGB, mImage->width(), mImage->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, mImage->data());
	glTexParameterf(mTarget, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameterf(mTarget, GL_TEXTURE_MAG_FILTER, m_magFilter);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, m_wrap);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, m_wrap);
	glBindTexture  (mTarget, 0);	//unbind
}

void Texture::activate(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(mTarget, mTextureId);
}

void Texture::deactivate()
{
	glBindTexture(mTarget, 0);
}


