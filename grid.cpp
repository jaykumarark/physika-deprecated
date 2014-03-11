#include "grid.h"


Grid::Grid(std::string filename)
{
	init(filename);
}


void Grid::init(std::string filename)
{
	//init height field
	m_img = new Image(filename);
	m_img->makeHeightMap();

	//init grid
	initGrid();

	//init Buffers
	initBuffers();
}

void Grid::initGrid()
{
	int gridX = m_img->width(); 
	int gridY = m_img->height();
	int gSize = 20;
	for(int j = 0; j < gridY; j++)
	{
		for(int i = 0; i < gridX; i++)
		{
			glm::vec3 v  = glm::vec3(i*gSize, -500*m_img->getColorAt(i, j), -j*gSize);
			glm::vec3 c = glm::vec3 ((float)i/128,  m_img->getColorAt(i, j), (float)j/128);
			m_colors.push_back(c);
			m_vertices.push_back(v);

		}
	}
	//create indices for the grid

	for( int j = 0; j < gridY-1 ; j++)
	{

		for( int i = 0; i < gridX-1; i++)
		{
			int v1 = i + j*gridX;
			int v2 = (i + 1) + j*gridX;
			int v6 = i+ (j+1)*gridX;
			int v7 = (i+1)+((j+1)*gridX);


			m_indices.push_back(v1);
			m_indices.push_back(v6);
			m_indices.push_back(v7);
			
			m_indices.push_back(v1);
			m_indices.push_back(v7);
			m_indices.push_back(v2);
			
		}
	}

	//calculate normals for the each face
	for( int j = 0; j < gridY-1 ; j++)
	{

		for( int i = 0; i < gridX-1; i++)
		{
			int v1 = i + j*gridX;
			int v2 = (i + 1) + j*gridX;
			int v6 = i+ (j+1)*gridX;

			glm::vec3 vertex1 = m_vertices[v1];
			glm::vec3 vertex2 = m_vertices[v2];
			glm::vec3 vertex6 = m_vertices[v6];

			glm::vec3 vector1 = vertex1 - vertex6; 
			glm::vec3 vector2 = vertex6 - vertex2; 

			m_normals.push_back(glm::cross(vector1, vector2));			
		}
	}
}

void Grid::initBuffers()
{
	//Generate buffers for grid vertices
	glGenBuffers(1, &m_vb);
	glBindBuffer(GL_ARRAY_BUFFER, m_vb);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Generate Buffers for grid Colors; 
	glGenBuffers(1, &m_cb);
	glBindBuffer(GL_ARRAY_BUFFER, m_cb);
	glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec3), &m_colors[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//generate buffers for grid indices
	glGenBuffers(1, &m_ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//generate buffers for grid normals 
	glGenBuffers(1, &m_nb);
	glBindBuffer(GL_ARRAY_BUFFER, m_nb);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Grid::render(int pos, int color, int normal)
{
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBindBuffer(GL_ARRAY_BUFFER, m_vb);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_cb);
	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_nb);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_TRUE, 0, 0);
	
	glFrontFace(GL_CW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
	glDrawElements(GL_TRIANGLES,m_indices.size(), GL_UNSIGNED_SHORT, (void*)0);
}

Grid::~Grid(void)
{
}
