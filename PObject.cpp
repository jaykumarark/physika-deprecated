#include "PObject.h"
#define GLM_SWIZZLE


PObject::PObject(glm::vec3 p,
				 std::string modelFile, 
				 std::string textureFile,
				 std::string vertexFile, 
				 std::string fragmentFile,
				 glm::vec3 a, 
				 glm::vec3 d, 
				 glm::vec3 s)
{
	m_objLoader = new ModelLoader(modelFile);
	m_vbo = new VertexBufferObject(GL_ARRAY_BUFFER, GL_TRIANGLES);
	m_tex = new Texture(GL_TEXTURE_2D, textureFile, GL_REPEAT, GL_LINEAR, GL_LINEAR);
	m_vertexFile = vertexFile;
	m_fragmentFile = fragmentFile;
	m_model = glm::translate(glm::mat4(1.0), p);
	init();
	setMaterial(a, d, s);

}

PObject::~PObject(void)
{
	delete m_vbo;
	delete m_shader;
	delete m_objLoader;
	delete m_tex;
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
	m_shader = new GLSLShader(m_vertexFile.c_str(), m_fragmentFile.c_str());
	m_model = glm::scale(m_model,glm::vec3(1, 1, 1));
	
}

void PObject::render(Camera cam, TrackBall* tb, Light* light)
{
	Light::LightProperties lprops = light->properties();

	//Light Position
	glm::vec4 lp = glm::vec4(light->position(), 1.f); 

	m_shader->use();

	//Setup Texture 
	m_tex->activate(GL_TEXTURE0);
	m_shader->setSampler("TextureSample2D",0);

	glm::mat4 m = cam.matrix() * m_model;
	glm::mat4 normalMatrix = glm::mat4(1);

	//Setting up Matrices
	m_shader->setUniform("ModelViewMatrix", cam.view()*m_model);	//uniform mat4 ModelViewMatrix;
	m_shader->setUniform("mvp",m);									//uniform mat4 mvp;			
	m_shader->setUniform("ViewMatrix", cam.view());				//uniform mat4 NormalMatrix;

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

	m_tex->deactivate();
	m_shader->disuse();
}