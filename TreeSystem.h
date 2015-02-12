#pragma once

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "camera.h"

#include <vector>


struct BranchNode
{
	glm::vec3 o; 
	glm::vec3 d; 
	float	  l; 
};


class TreeSystem
{
public:
	TreeSystem(void);
	void init();
	void render(Camera cam);
	void writeRules();
	~TreeSystem(void);
private:
	glm::mat4 m_model;
	BranchNode root; 
	std::vector<glm::vec3> points; 
	int m_generations; 
};

