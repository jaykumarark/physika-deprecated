#include<iostream>

float randomize(float min, float max)
{
	return float(((std::rand() / float(RAND_MAX)) * max) - min);
}

glm::vec3 randVec(glm::vec3 min, glm::vec3 max)
{
	glm::vec3 v; 
	if(min.x < 0)
		min.x *= -1;
	if(min.y < 0)
		min.y *= -1;
	if(min.z < 0)
		min.z *= -1;

	v.x = (((std::rand() / float(RAND_MAX) * max.x*2) - min.x));
	v.y = (((std::rand() / float(RAND_MAX) * max.y*2) - min.y));
	v.z = (((std::rand() / float(RAND_MAX) * max.y*2) - min.z));


	return v;
}