#include "PObject.h"
#define GLM_SWIZZLE


PObject::PObject(std::string filename, glm::vec3 a, glm::vec3 d, glm::vec3 s)
{
	m_objLoader = new ModelLoader(filename);
	m_vbo = new VertexBufferObject(GL_ARRAY_BUFFER, GL_TRIANGLES);
	init();
	setMaterial(a, d, s);
}

PObject::~PObject(void)
{
	delete m_vbo;
	delete m_shader;
	delete m_objLoader;
}

void PObject::setMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s)
{
	m_material.Ka = a; 
	m_material.Kd = d; 
	m_material.Ks = s; 
}

void PObject::init()
{
	m_vbo->init(m_objLoader->data(), m_objLoader->indices());
	m_shader = new GLSLShader("vs.glsl", "fs.glsl");
	m_model = glm::mat4(1.f);
	
}

void PObject::render(Camera cam, TrackBall* tb, Light* light)
{
	Light::LightProperties lprops = light->properties();

	//Light Position
	glm::vec4 lp = glm::vec4(light->position(), 1.f); 

	m_shader->use();
	glm::mat4 m = cam.matrix() * tb->matrix() * m_model;
	glm::mat4 normalMatrix = glm::transpose(cam.view() * tb->matrix() * m_model);

	//Setting up Matrices
	m_shader->setUniform("ProjectionMatrix", cam.projection());		//uniform mat4 ProjectionMatrix; 
	m_shader->setUniform("ModelViewMatrix", cam.view() * m_model);	//uniform mat4 ModelViewMatrix;
	m_shader->setUniform("mvp",m);									//uniform mat4 mvp;			
	m_shader->setUniform("NormalMatrix", normalMatrix);				//uniform mat4 NormalMatrix;

	//Light Position
	m_shader->setUniform("lightPosition", lp);
	
	//Setup Material 
	m_shader->setUniform("ka", m_material.Ka);
	m_shader->setUniform("kd", m_material.Kd);
	m_shader->setUniform("ks", m_material.Ks);

	//Setup Lights
	m_shader->setUniform("la", lprops.Ia);
	m_shader->setUniform("ld", lprops.Id);
	m_shader->setUniform("ls", lprops.Is);
	
	m_vbo->render();

	m_shader->disuse();
}