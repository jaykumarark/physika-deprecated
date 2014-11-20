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
	m_model = glm::scale(m_model,glm::vec3(30,30,30));
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
	m_model = glm::scale(m_model,glm::vec3(0.2, 0.2, .2));
	
}

void PObject::render(ACamera* cam, TrackBall* tb, Light* light)
{
	//Light Position
	glm::vec4 lp = glm::vec4(light->position(), 1.f); 
	Light::LightProperties lprops = light->properties();

	m_shader->use();

	//Setup Texture 
	m_tex->activate(GL_TEXTURE0);
	m_shader->setSampler("TextureSample2D",0);

	//Setting up Matrices
	glm::mat4 m = cam->matrix() * m_model;
	glm::mat3 NormalMatrix = glm::mat3(cam->view()*m_model);
	NormalMatrix = glm::transpose(glm::inverse(NormalMatrix));


	m_shader->setUniform("ModelViewMatrix", cam->view()*m_model);	//uniform mat4 ModelViewMatrix;
	m_shader->setUniform("mvp",m);									//uniform mat4 mvp;			
	m_shader->setUniform("ViewMatrix", cam->view());				//uniform mat4 NormalMatrix;
	m_shader->setUniform("NormalMatrix", NormalMatrix);

	m_shader->setUniform("EyePositionInWorld", cam->position());

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

	/*glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(cam.projection()));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(cam.view()*m_model));

	std::vector<ModelLoader::Vertex> data = m_vbo->data();
	glColor3f(1,0,0);
	glPointSize(3.0);

	glBegin(GL_POINTS);
	for(int i = 0 ; i < data.size(); i++ )
	{
	glVertex3f(data[i].p.x,data[i].p.y,data[i].p.z);
	}
	glEnd();
	glColor3f(0,0.8,1);
	glBegin(GL_LINES);
	for(int i = 0 ; i < data.size(); i++ )
	{
	glVertex3f(data[i].p.x,data[i].p.y,data[i].p.z);
	glm::vec3 p2 = data[i].p+0.3f*data[i].n;
	glVertex3f(p2.x,p2.y,p2.z);
	}
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
}