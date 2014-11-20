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

void PickingRay::render(ACamera* cam)
{
	glColor3f(0.f, 1.f, 0.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(cam->projection()));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(cam->view()));

	glBegin(GL_LINES);

	glVertex3f(m_clickpos.x, m_clickpos.y, m_clickpos.z);
	glVertex3f(m_clickEnd.x, m_clickEnd.y, m_clickEnd.z);
	
	glEnd();

	glPointSize(10.f);
	glBegin(GL_POINTS);
	glColor3f(1.f, 1.f, 1.f);
	glVertex3f(m_clickpos.x, m_clickpos.y, m_clickpos.z);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(m_clickEnd.x, m_clickEnd.y, m_clickEnd.z);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//source: http://schabby.de/picking-opengl-ray-tracing/
void PickingRay::getRay(float mx, float my, ACamera* cam)
{
	glm::vec2 m;
	////clip
	m.x = (2*(float)mx - float(mW)) / float(mW);
	m.y = (float(mH)- 2*(float)my)/float(mH);

	glm::vec3 view = cam->forward();
	glm::vec3 h = glm::normalize(glm::cross(view, cam->up()));
	glm::vec3 v = glm::normalize(glm::cross(h, view));

	float rad = cam->mFov * M_PI / 180; 
	float vLength = tan(rad / 2.f) * cam->mNear;
	float hLength = vLength * cam->mAsp;

	v *= vLength; 
	h *= hLength;

	glm::vec3 pos = cam->position() + (view * cam->mNear) + (h*m.x) + (v*m.y);
	glm::vec3 dir = glm::normalize(pos - cam->position());

	m_clickpos = pos;
	m_clickEnd = cam->forward();

	m_rayPos = pos; 
	m_rayDir = dir;
}


void PickingRay::print(glm::vec3 p)
{
	std::cout<<"x:"<<p.x<<" y:"<<p.y<<" z:"<<p.z<<std::endl;
}


//Ray-Triangle Intersection 
//source: http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-9-ray-triangle-intersection/m-ller-trumbore-algorithm/
bool PickingRay::intersect(glm::vec3 v0f, glm::vec3 v1f, glm::vec3 v2f, float mx, float my, ACamera* cam, glm::mat4 m)
{
	glm::vec4 temp0 = m * glm::vec4(v0f.x, v0f.y, v0f.z, 1);
	glm::vec4 temp1 = m * glm::vec4(v1f.x, v1f.y, v1f.z, 1);
	glm::vec4 temp2 = m * glm::vec4(v2f.x, v2f.y, v2f.z, 1);

	glm::vec3 v0 = glm::vec3(temp0.x, temp0.y, temp0.z);
	glm::vec3 v1 = glm::vec3(temp1.x, temp1.y, temp1.z);
	glm::vec3 v2 = glm::vec3(temp2.x, temp2.y, temp2.z);

	getRay(mx, my, cam);
	//soren

	float eps = 0.000000001f;

	glm::vec3 e1 = v1 - v0;
	glm::vec3 e2 = v2 - v0;

	glm::vec3 p = glm::cross(m_rayDir, e2);
	float a = glm::dot(e1, p);

	if(a > -eps && a < eps)
	{
		return false;
	}

	float f = 1.0f / a;
	glm::vec3 s = m_rayPos - v0;
	float u = f * glm::dot(s,p);

	if(u < 0.0f || u > 1.0f)
		return false;

	glm::vec3 q = glm::cross(s, e1);
	float v = f * glm::dot(m_rayDir, q);

	if(v < 0.0f || u+v > 1.0f)
		return false;

	float t = f * glm::dot(e2, q);

	return true;
}

glm::vec3 PickingRay::rayPos()
{
	return m_rayPos;
}

glm::vec3 PickingRay::rayDir()
{
	return m_rayDir;
}