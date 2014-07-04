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
	m_shader = new GLSLShader("vs.glsl", "fs.glsl");
	m_model = glm::mat4(1.f);
}

void PObject::render(Camera cam, TrackBall* tb)
{
	m_shader->use();
	glm::mat4 m = cam.matrix() * tb->matrix() * m_model;
	glm::mat4 normalMatrix = glm::transpose(cam.view() * tb->matrix() * m_model);

	//Setting up Matrices
	m_shader->setUniform("ProjectionMatrix", cam.projection());		//uniform mat4 ProjectionMatrix; 
	m_shader->setUniform("ModelViewMatrix", cam.view() * m_model);	//uniform mat4 ModelViewMatrix;
	m_shader->setUniform("mvp",m);									//uniform mat4 mvp;			
	m_shader->setUniform("NormalMatrix", normalMatrix);				//uniform mat4 NormalMatrix;

	//Light Position
	glm::vec4 lp = cam.view() * m_model * glm::vec4(0,50,0,1);
	m_shader->setUniform("lightPosition", lp);
	//Material 
	glm::vec3 kd = glm::vec3(1.f, 1.f, 1.f);
	m_shader->setUniform("kd", kd);
	//Light intensity
	glm::vec3 ld = glm::vec3(1.f, 1.f, 1.f);
	m_shader->setUniform("ld", ld);

	glPointSize(3.0f);
	m_vbo->render();

	m_shader->disuse();
}