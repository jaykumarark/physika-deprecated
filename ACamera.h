#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


class ACamera
{
public:
	ACamera();
	glm::vec3 up();
	glm::vec3 forward();
	void SetLookAt(glm::vec3 la);
	void SetPosition(glm::vec3 p);
	void SetVelocity(float v);
	void SetWindowSize(int w, int h);
	void SetZoom(float zoom);
	void SetParams(float fov, float near, float far, float asp);	// fov, near, far, asp
	void onMouseMove(int x, int y);
	void onMouseDown(int x, int y);
	void onMouseUp(int x, int y);
	void onMouseWheel(float direction);
	glm::mat4 view();
	glm::mat4 matrix();
	glm::vec3 position();
	glm::mat4 rotation();
	glm::mat4 projection();
	
	float mFov;
	float mNear; 
	float mFar; 
	float mAsp;

private: 
	glm::vec3 mPos;
	glm::vec3 mLookAt; 
	glm::vec2 mBaseRotation; 
	glm::vec2 mNewRotation;
	glm::vec2 mOld;			//old mouse point
	float mVel;
	float mWinWidth; 
	float mWinHeight;
	bool  isMouseDown;
	float mZoom;
};

