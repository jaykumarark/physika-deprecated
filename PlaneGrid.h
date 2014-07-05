#pragma once

#include <GL/glew.h>
#include <glm\glm.hpp>
#include "image.h"
#include "texture.h"
#include "VertexBufferObject.h"
#include "GLSLShader.h"
#include <vector>
#include "PlaneGrid.h"
#include "Camera.h"
#include "trackball.h"


class PlaneGrid
{
public:
	PlaneGrid(glm::vec3 p, float w, float h);
	void render(Camera cam, TrackBall* tb);
	~PlaneGrid(void);
	

private:
	void init();

	float mW, mH;
	glm::vec3 mPos;
	glm::mat4 m_model;
	Texture* mTex;
	GLSLShader* m_shader; 
	VertexBufferObject* m_vbo;
};

