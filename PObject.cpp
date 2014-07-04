#include "PObject.h"


PObject::PObject(std::string filename)
{
	m_objLoader = new ModelLoader(filename);
	m_vbo = new VertexBufferObject(GL_ARRAY_BUFFER, GL_TRIANGLES);
	init();
}

PObject::~PObject(void)
{
	delete m_vbo;
	delete m_shader;
	delete m_objLoader;
}

void PObject::init()
{
	m_vbo->init(m_objLoader->data(), m_objLoader->indices());
	m_shader = new GLSLShader("diffuseVS.glsl", "diffuseFS.glsl");
}

void PObject::render(Camera cam, TrackBall* tb, glm::mat4 model)
{


	m_vbo->render();
}