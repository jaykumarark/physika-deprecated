#include "VertexBufferObject.h"


VertexBufferObject::VertexBufferObject(GLenum target, GLenum primitiveMode)
{
	m_target = target; 
	m_primitiveMode = primitiveMode;
}

void VertexBufferObject::init(const std::vector<ModelLoader::Vertex> verts, const std::vector<unsigned int> indices)
{
	m_verts = verts;
	m_indices = indices;
	createVBO();
}

std::vector<ModelLoader::Vertex> VertexBufferObject::data()
{
	return m_verts;
}

void VertexBufferObject::createVBO()
{

	glGenBuffers(1, &m_vbId);
	glBindBuffer(m_target, m_vbId);
	glBufferData(m_target, sizeof(ModelLoader::Vertex)*m_verts.size(), &m_verts[0], GL_STATIC_DRAW);
	glBindBuffer(m_target, 0);

	//Index buffers
	glGenBuffers(1, &m_ibId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexBufferObject::render()
{
	glEnableVertexAttribArray(VERTEX_POSITION);
	glEnableVertexAttribArray(VERTEX_NORMAL);
	glEnableVertexAttribArray(VERTEX_TEXTURE);
	glEnableVertexAttribArray(VERTEX_COLOR);

	glBindBuffer(m_target, m_vbId);
	glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ModelLoader::Vertex), 0);
	glVertexAttribPointer(VERTEX_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(ModelLoader::Vertex), (const GLvoid*)12);
	glVertexAttribPointer(VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ModelLoader::Vertex), (const GLvoid*)20);
	glVertexAttribPointer(VERTEX_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(ModelLoader::Vertex), (const GLvoid*)32);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibId);

	glDrawElements(m_primitiveMode, m_indices.size(), GL_UNSIGNED_INT, 0);


	glDisableVertexAttribArray(VERTEX_POSITION);
	glDisableVertexAttribArray(VERTEX_NORMAL);
	glDisableVertexAttribArray(VERTEX_TEXTURE);
	glDisableVertexAttribArray(VERTEX_COLOR);
}

VertexBufferObject::~VertexBufferObject(void)
{
}
