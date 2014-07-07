#pragma once
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	bool isMouseDown;
	glm::vec3 mpos;
	float mhAngle; 
	float mvAngle; 
	float mFov; 
	float mNear; 
	float mFar;
	float mAsp; 
	float mVel;
	void normalizeAngles();
	int mWindowWidth;
	int mWindowHeight;
	glm::vec3 m_target;
	

public:
	int mMouseX;
	int mMouseY;

	Camera(void);
	~Camera(void);
	void init();

	const glm::vec3& position(); 
	void setPosition(const glm::vec3& position); 
	void offsetPosition(const glm::vec3& offset); 
	float fov();
	void setFov(float fov); 

	float nearPlane(); 
	float farPlane(); 

	glm::mat4 view(); 
	glm::mat4 projection();
	glm::mat4 matrix();
	const glm::vec3 up();
	const glm::vec3 right();
	const glm::vec3 forward();
	void setAspRatio(float aspRat);
	float viewportAspRat();
	void lookAt(glm::vec3 position);
	const glm::vec3 target();
	void offsetOrientation(float upAngle, float rightAngle);
	glm::mat4 orientation(); 
	void setNearFar(float near, float far);
	void setVelocity(float speed);
	float velocity();

	const float vAngle();
	const float hAngle();

	void setWindowCoords(int w, int h);
	void setMouseCoords(int x, int y);
	void Camera::onMouseMove(int x, int y);
	void Camera::onMouseDown();
	void Camera::onMouseUp();
	

};

