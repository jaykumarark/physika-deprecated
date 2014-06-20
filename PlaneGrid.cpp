#include "PlaneGrid.h"


PlaneGrid::PlaneGrid(glm::vec3 p, float w, float h)
{
	mPos = p;
	mW = w;
	mH = h;
	mTex = new Texture(GL_TEXTURE_2D, "textures/gridtexture128.jpg");
	init();
	initBuffers();
}

void PlaneGrid::init()
{
	glm::vec3 p1 = glm::vec3(mPos.x-(mW/2), 0, mPos.z-(mH/2));
	glm::vec3 p2 = glm::vec3(mPos.x-(mW/2), 0, mPos.z+(mH/2));
	glm::vec3 p3 = glm::vec3(mPos.x+(mW/2), 0, mPos.z+(mH/2));
	glm::vec3 p4 = glm::vec3(mPos.x+(mW/2), 0, mPos.z-(mH/2));

	m_verts.push_back(p1);
	m_verts.push_back(p2);
	m_verts.push_back(p3);
	m_verts.push_back(p4);

	glm::vec2 t1 = glm::vec2(p1.x/mW, p1.z/mH);
	glm::vec2 t2 = glm::vec2(p2.x/mW, p2.z/mH);
	glm::vec2 t3 = glm::vec2(p3.x/mW, p3.z/mH);
	glm::vec2 t4 = glm::vec2(p4.x/mW, p4.z/mH);

	m_tcoords.push_back(t1);
	m_tcoords.push_back(t2);
	m_tcoords.push_back(t3);
	m_tcoords.push_back(t4);

	//push indices
	//triangle 1
	m_indices.push_back(0);
	m_indices.push_back(1);
	m_indices.push_back(2);

	//triangle 2
	m_indices.push_back(0);
	m_indices.push_back(2);
	m_indices.push_back(3);
	

}

void PlaneGrid::initBuffers()
{
	//Generate buffers for vertices
	glGenBuffers(1, &m_vb);
	glBindBuffer(GL_ARRAY_BUFFER, m_vb);
	glBufferData(GL_ARRAY_BUFFER, m_verts.size()*sizeof(glm::vec3), &m_verts[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//generate buffers for indices
	glGenBuffers(1, &m_ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Texcoords;
	glGenBuffers(1, &m_tb);
	glBindBuffer(GL_ARRAY_BUFFER, m_tb);
	glBufferData(GL_ARRAY_BUFFER, m_tcoords.size()*sizeof(glm::vec2), &m_tcoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void PlaneGrid::render(int pos, int color, int normal, int tex_loc, int sample)
{
	mTex->activate(sample, GL_TEXTURE0, 0);

	glPolygonMode( GL_FRONT, GL_LINE );
	glBindBuffer(GL_ARRAY_BUFFER, m_vb);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_tb);
	glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glFrontFace(GL_CW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, (void*)0);
	
}

PlaneGrid::~PlaneGrid(void)
{
}
