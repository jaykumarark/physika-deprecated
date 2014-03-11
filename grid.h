#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm\glm.hpp>
#include "image.h"

class Grid
{
public:
	Grid(std::string filename);
	void init(std::string filename);
	void initGrid();
	void initBuffers();
	void render(int pos, int color, int normal);
	~Grid(void);
private:
	std::vector<glm::vec3> m_vertices; 
	std::vector<glm::vec3> m_colors;
	std::vector<glm::vec3> m_normals;
	std::vector<unsigned short> m_indices; 

	GLuint m_vb;				//Vertex Buffers; 
	GLuint m_cb;				//Color Buffers; 
	GLuint m_ib;				//Indices Buffers; 
	GLuint m_nb;				//Normal buffers;

	Image *m_img; 

};

