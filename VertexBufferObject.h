#pragma once

#include <gl\glew.h>
#include "ModelLoader.h"
#include <vector>

#define VERTEX_POSITION		0
#define VERTEX_NORMAL		1
#define VERTEX_TEXTURE		2
#define VERTEX_COLOR		3


class VertexBufferObject
{
public:
	VertexBufferObject(GLenum m_target, GLenum m_primitiveMode);
	void init(const std::vector<ModelLoader::Vertex> verts, const std::vector<unsigned int> indices);
	
	void setData();
	void render();
	~VertexBufferObject(void);


private:
	//private Methods
	void createVBO();


	//private Variables
	GLenum m_target;
	GLenum m_primitiveMode;
	std::vector<ModelLoader::Vertex> m_verts;
	std::vector<unsigned int> m_indices;
	GLuint	m_vbId;
	GLuint	m_ibId;

};

