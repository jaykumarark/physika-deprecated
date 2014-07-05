#pragma once

#include <glm/glm.hpp>
#include <gl/glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "Camera.h"

class PickingRay
{
public:
	PickingRay(int width, int height);
	void getRay(int mx, int my, Camera cam);
	void render(Camera cam);
	~PickingRay(void);
private:
	int mW, mH;
	glm::vec3 m_clickpos;
	glm::vec3 m_clickEnd;
};

