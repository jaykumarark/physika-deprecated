#include "Light.h"
#define GLM_SWIZZLE

using namespace glm;

Light::Light(vec3 p, vec3 a, vec3 d, vec3 s)
{
	m_pos = p;
	m_objLoader = new ModelLoader("models/sphere.obj");
	m_vbo = new VertexBufferObject(GL_ARRAY_BUFFER, GL_TRIANGLES);
	m_angle = 0.f;
	init();
	setProperty(a, d, s);
}

Light::~Light(void)
{
	delete m_shader;
	delete m_vbo; 
	delete m_objLoader;
}

glm::vec3 Light::position()
{
	return (m_model * glm::vec4(m_pos, 1)).swizzle(X,Y,Z);
	//return m_pos;
}

Light::LightProperties Light::properties()
{
	return m_props;
}

void Light::update()
{
	m_angle += 0.5; 
	
}

void Light::setProperty(glm::vec3 a, glm::vec3 d, glm::vec3 s)
{
	m_props.Ia = a;
	m_props.Id = d;
	m_props.Is = s;
}

void Light::init()
{
	m_vbo->init(m_objLoader->data(), m_objLoader->indices());
	m_shader = new GLSLShader("LightVS.glsl", "LightFS.glsl");	
}

void Light::render(Camera cam, TrackBall* tb)
{
	//glPolygonMode(GL_FRONT, GL_LINE);
	m_shader->use();
	m_model = glm::mat4(1);
	m_model = glm::rotate(m_model, m_angle, glm::vec3(0, 1, 0));
	m_model = glm::translate(m_model, m_pos);
	glm::mat4 m = cam.matrix() * m_model;

	//Setup Matrices
	m_shader->setUniform("mvp", m);
	
	m_vbo->render();

	m_shader->disuse();

}