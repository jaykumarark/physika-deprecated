#pragma once

#include <glm/glm.hpp>

class PickingRay
{
public:
	PickingRay(int width, int height);
	void getRay(int mx, int my);
	~PickingRay(void);
private:
	int mW, mH;
};

