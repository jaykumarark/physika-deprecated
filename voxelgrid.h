#pragma once
#include<GL\glew.h>
#include<iostream>
#include<vector>
#include<glm\glm.hpp>
#define idx(i,j,k,len, area) j+(i*len)+(k*area)

class VoxelGrid
{
private:
	int m_width, m_height, m_depth; // Preferably multiples of 2 like 1024, 512, etc. 
	int m_side; // side of one voxel. Preferably multiples of 2 like 4 or 6. 
	std::vector<glm::vec3> m_vertices;
	std::vector<unsigned short> m_indices; 

	GLuint m_vb;
	GLuint m_ib;
	
public:
	VoxelGrid(int w, int h, int d, int s);
	~VoxelGrid(void);
	void createGrid();
	void createBuffers();
	void render(int pos);
};

