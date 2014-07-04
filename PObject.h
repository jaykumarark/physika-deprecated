#pragma once

#include "GLSLShader.h"
#include "VertexBufferObject.h"
#include "ModelLoader.h"
#include "Camera.h"
#include "trackball.h"

class PObject
{
public:
	PObject(std::string filename);
	void init();
	void render(Camera cam, TrackBall* tb);
	~PObject(void);

private:
	GLSLShader* m_shader;
	VertexBufferObject* m_vbo;
	ModelLoader* m_objLoader;
	glm::mat4 m_model;		//model matrix is specific to each object and not global
};

