#include "texture.h"


Texture::Texture(GLenum TextureTarget, string filename)
{
	mTarget = TextureTarget;
	mFilename = filename;
	mImage = NULL;
	init();
	load();
}

void Texture::init()
{
	glGenTextures(1, &mTextureId);
	glBindTexture(mTarget, mTextureId);
	mImage = new Image(mFilename);
}

bool Texture::load()
{
	glTexImage2D(mTarget, 0, GL_RGB, mImage->width(), mImage->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, mImage->data());
	glTexParameterf(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return true;			//change this later
}

void Texture::activate(int location, GLenum textureUnit, GLushort unit)
{
	glActiveTexture(textureUnit);
	glBindTexture(mTarget, mTextureId);
	glUniform1i(location, unit);
};

Texture::~Texture(void)
{
}
