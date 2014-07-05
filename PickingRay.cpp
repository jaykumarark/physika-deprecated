#include "PickingRay.h"


PickingRay::PickingRay(int width, int height)
{
	mW = width; 
	mH = height;
	m_clickEnd = m_clickpos = glm::vec3(0);
}
PickingRay::~PickingRay(void)
{
}

void PickingRay::render(Camera cam)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(cam.projection()));

	glm::mat4 m = glm::scale(glm::mat4(1), glm::vec3(100, 100, 100));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(cam.view() * m));

	glBegin(GL_LINES);

	glVertex3f(m_clickpos.x, m_clickpos.y, m_clickpos.z);
	glVertex3f(m_clickEnd.x, m_clickEnd.y, m_clickEnd.z);

	glEnd();


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
	m_clickpos = glm::vec3(ray_world.x, ray_world.y, ray_world.z);
	m_clickEnd = m_clickpos;
	m_clickEnd.z = -20;
	ray_world.w = 0;
	ray_world = glm::normalize(ray_world);

}

