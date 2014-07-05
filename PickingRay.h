#pragma once

#include <glm/glm.hpp>

#include <glm/gtx/transform.hpp>
#include "Camera.h"

class PickingRay
{
public:
	PickingRay(int width, int height);
	void getRay(int mx, int my, Camera cam);
	~PickingRay(void);
private:
	int mW, mH;
};

