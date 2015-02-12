#include "TreeSystem.h"

using namespace glm;

TreeSystem::TreeSystem(void)
{
	m_model = glm::mat4(1);
	m_generations = 5;
	init();
}

void TreeSystem::init()
{
	root.o = glm::vec3(0,0,0);
	root.d = glm::vec3(0, 1, 0);
	root.l = 4.f; 
}

void TreeSystem::render(Camera cam)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(cam.projection()));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(cam.view() * m_model));

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	for(int i = 0 ; i < points.size(); i+=2)
	{
		glVertex3f(points[i].x, points[i].y, points[i].z);
		glVertex3f(points[i+1].x, points[i+1].y, points[i+1].z);
	}
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


}

void TreeSystem::writeRules()
{
	BranchNode left; 
	BranchNode right; 
	glm::vec3 start = root.o + root.d * root.l;
	glm::vec4 temp;		//left's dir
	glm::vec3 ldir; 
	glm::vec3 rdir; 
	
	glm::vec4 dir = glm::vec4(root.d.x, root.d.y, root.d.z, 0);

	temp = glm::rotate(dir, 45.f, glm::vec3(0,0,1));
	ldir = glm::vec3(temp.x, temp.y, temp.z);

	temp = glm::rotate(dir, -45.f, glm::vec3(0,0,1));
	rdir = glm::vec3(temp.x, temp.y, temp.z);

	right.o = start; 
	right.l = root.l/2; 
	right.d = rdir;

	left.o = start; 
	left.l = root.l/2; 
	left.d = ldir;

	points.push_back(root.o);
	points.push_back(start);

	//left
	glm::vec3 end = left.o + left.d * left.l;
	points.push_back(start);
	points.push_back(end);
	
	end = right.o + right.d * right.l;
	//right
	points.push_back(start);
	points.push_back(end);

}

TreeSystem::~TreeSystem(void)
{

}
