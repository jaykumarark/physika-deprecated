#pragma once

#include "GLSLShader.h"
#include "VertexBufferObject.h"
#include "ModelLoader.h"
#include "Camera.h"


class PObject
{
public:
	PObject(std::string filename);
	void init();
	void render(Camera cam);
	~PObject(void);

private:
	GLSLShader* m_shader;
	VertexBufferObject* m_vbo;
	ModelLoader* m_objLoader;
};

