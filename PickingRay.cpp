#include "PickingRay.h"


PickingRay::PickingRay(int width, int height)
{
	mW = width; 
	mH = height;
}

void PickingRay::getRay(int mx, int my)
{
	glm::vec3 v;
	v.x = (2*(float)mx - float(mW)) / float(mW);
	v.y = (float(mH)- 2*(float)my)/float(mH);
}

PickingRay::~PickingRay(void)
{
}
