#pragma once

#include <GL/glew.h>
#include <glm\glm.hpp>
#include "image.h"
#include "texture.h"
#include <vector>


class PlaneGrid
{
public:
	PlaneGrid(glm::vec3 p, float w, float h);
	void render(int pos, int color, int normal, int tex_loc, int sample);
	~PlaneGrid(void);

private:
	void init();
	void initBuffers();


	GLuint m_vb;				//Vertex Buffers; 
	GLuint m_ib;				//Indices Buffers; 
	GLuint m_tb;				//Texture Coords Buffers;

	glm::vec3 mPos;
	float mW;
	float mH;
	Texture* mTex;
	std::vector<glm::vec3> m_verts;
	std::vector<glm::vec2> m_tcoords;
	std::vector<unsigned short> m_indices;
};

