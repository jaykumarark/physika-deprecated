#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>

using namespace std;

class TrackBall
{

private:
	int mWidth;
	int mHeight;
	
	float rotateStep;
	bool isDown;
	glm::mat4 m_rotate;
	
	glm::mat4 computeCurrRot();
	glm::vec3 twoDto3D(int x, int y);
	void print(glm::vec3 v);
	float mag(glm::vec3);

	float lastAngle;
	float newAngle;

public:
	TrackBall(int width, int height);
	glm::mat4 matrix();
	void mouseDown(int x, int y);
	void mouseMove(int x, int y);
	void mouseUp();
	~TrackBall(void);

	glm::vec3 currPoint; 
	glm::vec3 m_axis;
	glm::vec3 lastPoint;
};

