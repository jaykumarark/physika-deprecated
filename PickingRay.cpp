#include "PickingRay.h"


PickingRay::PickingRay(int width, int height)
{
	mW = width; 
	mH = height;
}

void PickingRay::getRay(int mx, int my, Camera cam)
{
	glm::vec4 v;
	//clip
	v.x = (2*(float)mx - float(mW)) / float(mW);
	v.y = (float(mH)- 2*(float)my)/float(mH);
	v.z = -1.f; 
	v.w = 1.f; 

	glm::vec4  ray_eye = glm::inverse(cam.projection()) * v; 
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.f, 0.f);
	glm::vec4 ray_world = (glm::inverse(cam.view()) * ray_eye);
	ray_world.w = 0;
	ray_world = glm::normalize(ray_world);

}

PickingRay::~PickingRay(void)
{
}
