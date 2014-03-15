#include "voxelgrid.h"



VoxelGrid::VoxelGrid(int w, int h, int d, int s)
{
	m_width = w; 
	m_height = h; 
	m_depth = d; 
	m_side = s;
	createGrid();
	createBuffers();
}

void VoxelGrid::createGrid()
{
	int xpts = m_width/m_side;
	int ypts = m_height/m_side;
	int zpts = m_depth/m_side;

	int len = xpts+1;
	int area = (xpts+1) * (ypts+1);

	//generate vertices
	for(int k = 0 ; k <= zpts; k++)
	{
		for (int i = 0; i <= xpts;i++)
		{
			for(int j = 0; j <=ypts; j++)
			{
				glm::vec3 v = glm::vec3(i*m_side, -j*m_side, -k*m_side);
				std::cout<<"("<<i<<","<<j<<","<<k<<") = "<<idx(i, j, k, len, area)<<std::endl;
				m_vertices.push_back(v);
			}
		}
	}

	//generate indices
	for(int k = 0 ; k <= zpts-1; k++)
	{
		for (int i = 0; i <= xpts-1;i++)
		{
			for(int j = 0; j <=ypts-1; j++)
			{
				int v1 = idx(i, j, k, len, area);
				int v2 = idx(i, j+1, k, len, area);
				int v6 = idx(i+1, j, k, len, area);
				int v7 = idx(i+1, j+1, k, len, area);
				int v21 = idx(i,j,k+1, len, area);
				int v22 = idx(i,j+1,k+1, len, area);
				int v26 = idx(i+1,j,k+1, len, area);
				int v27 = idx(i+1,j+1,k+1, len, area);

				//front
				m_indices.push_back(v1);
				m_indices.push_back(v7);
				m_indices.push_back(v2);
				
				m_indices.push_back(v1);
				m_indices.push_back(v6);
				m_indices.push_back(v7);

				//top
				m_indices.push_back(v1);
				m_indices.push_back(v2);
				m_indices.push_back(v21);

				m_indices.push_back(v21);
				m_indices.push_back(v2);
				m_indices.push_back(v22);
				
				//right
				m_indices.push_back(v2);
				m_indices.push_back(v7);
				m_indices.push_back(v27);

				m_indices.push_back(v27);
				m_indices.push_back(v22);
				m_indices.push_back(v2);

				//back
				m_indices.push_back(v22);
				m_indices.push_back(v27);
				m_indices.push_back(v26);

				m_indices.push_back(v26);
				m_indices.push_back(v21);
				m_indices.push_back(v22);

				//bottom
				m_indices.push_back(v6);
				m_indices.push_back(v26);
				m_indices.push_back(v27);

				m_indices.push_back(v6);
				m_indices.push_back(v27);
				m_indices.push_back(v7);

				//left
				m_indices.push_back(v21);
				m_indices.push_back(v26);
				m_indices.push_back(v6);

				m_indices.push_back(v21);
				m_indices.push_back(v6);
				m_indices.push_back(v1);
			}
		}
	}

}

void VoxelGrid::createBuffers()
{
	glGenBuffers(1, &m_vb);
	glBindBuffer(GL_ARRAY_BUFFER, m_vb);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VoxelGrid::render(int pos)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_ARRAY_BUFFER, m_vb);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	//glDrawArrays(GL_POINTS, 0, m_vertices.size());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, (void*)0);
}

VoxelGrid::~VoxelGrid(void)
{
}
