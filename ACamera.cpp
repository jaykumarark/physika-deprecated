#include "ACamera.h"


ACamera::ACamera(void){
	mBaseRotation = glm::vec2(0);
	mNewRotation = glm::vec2(0);
	mOld = glm::vec2(0);
	isMouseDown = false;
}

void ACamera::SetLookAt(glm::vec3 la)		{mLookAt = la;}
void ACamera::SetPosition(glm::vec3 p)		{mPos = p;}
void ACamera::SetVelocity(float v)			{mVel = v;}
void ACamera::SetZoom(float zoom)			{mZoom = zoom;}
void ACamera::SetWindowSize(int w, int h)
{	
	mWinWidth  = w;
	mWinHeight = h; 
}

void ACamera::SetParams(float fov, float near, float far, float asp)
{
	mFov	= fov;
	mNear	= near; 
	mFar	= far; 
	mAsp	= asp;
}

glm::mat4 ACamera::view()
{
	glm::mat4 v = (glm::lookAt(glm::vec3(mPos.x, mPos.y, mZoom), mLookAt,glm::vec3(0,1,0))*rotation());
	return v;
}

glm::mat4 ACamera::projection()
{
	return glm::perspective(mFov, mAsp, mNear, mFar);
}

glm::vec3 ACamera::up()
{
	glm::vec3 p = glm::vec3(glm::inverse(view()) * glm::vec4(0,1,0,0));
	p = glm::normalize(p);
	return p;
}

glm::vec3 ACamera::forward()
{
	glm::vec3 p = glm::vec3(glm::inverse(view()) * glm::vec4(0,0,-1,0));
	p = glm::normalize(p);
	return p;
}

void ACamera::onMouseDown(int x, int y)
{
	isMouseDown = true;
	mOld.x = (float)x;
	mOld.y = (float)y;
}

void ACamera::onMouseMove(int x, int y)
{
	if(isMouseDown){
		glm::vec2 mouse_current;
		mouse_current.x = (float)x;
		mouse_current.y = (float)y;

		glm::vec2 dx = mouse_current-mOld;
		mNewRotation.x = dx.x / mWinWidth * 180.f;
		mNewRotation.y = dx.y / mWinHeight* 180.f;
	}
}

void ACamera::onMouseUp(int x, int y)
{
	mBaseRotation += mNewRotation;
	mNewRotation = glm::vec2(0.0);
	mOld.x = (float)x;
	mOld.y = (float)y;
	isMouseDown = false;
}

void ACamera::onMouseWheel(float direction)
{
	float delta = 5;
	delta*=direction;
	mZoom -= delta;
}

glm::mat4 ACamera::rotation()
{
	glm::mat4 M = glm::mat4(1.f);
	M = glm::rotate(M, mBaseRotation.y + mNewRotation.y, glm::vec3(1,0,0));
	M = glm::rotate(M, mBaseRotation.x + mNewRotation.x, glm::vec3(0,1,0));
	return M;
}

glm::mat4 ACamera::matrix()
{
	return (projection()*view());
}

glm::vec3 ACamera::position()
{
	glm::vec3 p = glm::vec3(glm::inverse(view()) * glm::vec4(0,0,0,1));
	return p;
}
