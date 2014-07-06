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

//source: http://schabby.de/picking-opengl-ray-tracing/
void PickingRay::getRay(float mx, float my, Camera cam)
{
	glm::vec2 m;
	////clip
	m.x = (2*(float)mx - float(mW)) / float(mW);
	m.y = (float(mH)- 2*(float)my)/float(mH);

	glm::vec3 view = glm::normalize(cam.target() - cam.position());
	glm::vec3 h = glm::normalize(glm::cross(view, cam.up()));
	glm::vec3 v = glm::normalize(glm::cross(h, view));

	float rad = cam.fov() * M_PI / 180; 
	float vLength = tan(rad / 2.f) * cam.nearPlane();
	float hLength = vLength * cam.viewportAspRat();

	v *= vLength; 
	h *= hLength;

	glm::vec3 pos = cam.position() + (view * cam.nearPlane()) + (h*m.x) + (v*m.y);
	glm::vec3 dir = glm::normalize(pos - cam.position());

	m_rayPos = pos; 
	m_rayDir = dir;
}


void PickingRay::print(glm::vec3 p)
{
	std::cout<<"x:"<<p.x<<" y:"<<p.y<<" z:"<<p.z<<std::endl;
}


//Ray-Triangle Intersection 
//source: http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-9-ray-triangle-intersection/ray-triangle-intersection-geometric-solution/
bool PickingRay::intersect(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float mx, float my, Camera cam)
{
	getRay(mx, my, cam);
	//glm::vec3 e1 = p1 - p0; 
	//glm::vec3 e2 = p2 - p0; 

	//glm::vec3 N = glm::cross(e1, e2);

	//float nDotRayDir = glm::dot(N, m_rayDir);
	////if parallel or not
	//if(nDotRayDir == 0)
	//	return false; //parallel

	////compute d
	//float d = glm::dot(N, p0);
	////compute t
	//float t = -(glm::dot(N, m_rayPos)+d);

	//if(t < 0)
	//	return false; 
	////intersection point 
	//glm::vec3 P = m_rayPos + t * m_rayDir;

	//glm::vec3 e0 = p0-p1;
	//e1 = p1-p2;
	//e2 = p2-p0;

	//glm::vec3 c0 = P - p0;
	//glm::vec3 c1 = P - p1;
	//glm::vec3 c2 = P - p2;

	//if (!(glm::dot(N, glm::cross(e0, c0)) > 0 && 
	//	glm::dot(N, glm::cross(e1, c1)) > 0 &&
	//	glm::dot(N, glm::cross(e2, c2)) > 0))
	//	{return false;}


	//return true;


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

	float t = (std::numeric_limits<float>::max)();
	t = f * glm::dot(e2, q);

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