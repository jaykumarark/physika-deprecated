#include "Skybox.h"


Skybox::Skybox(glm::vec3 p,
			   std::string modelFile, 
			   vector<std::string> textureFile, 
			   std::string vertexFile, 
			   std::string fragmentFile)
{
	m_objLoader = new ModelLoader(modelFile);
	m_vbo = new VertexBufferObject(GL_ARRAY_BUFFER, GL_TRIANGLES);
	m_tex = new CubeMap(textureFile, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
	m_vertexFile = vertexFile;
	m_fragmentFile = fragmentFile;
	m_model = glm::translate(glm::mat4(1.0), p);
	init();
	
}


Skybox::~Skybox(void)
{
	delete m_shader;
	delete	m_vbo;
	delete	m_objLoader;
	delete m_tex;
}


void Skybox::init()
{
	m_vbo->init(m_objLoader->data(), m_objLoader->indices());
	m_shader = new GLSLShader(m_vertexFile.c_str(), m_fragmentFile.c_str());
	m_model = glm::scale(m_model,glm::vec3(1024, 1024, 1024));
}


void Skybox::render(Camera cam)
{
	glPolygonMode(GL_BACK, GL_LINE);

	m_shader->use();

	//Setup Texture 
	m_tex->activate(GL_TEXTURE0);
	m_shader->setSampler("TextureSample3D",0);

	glm::mat4 m = cam.matrix() * m_model;
	glm::mat4 normalMatrix = glm::mat4(1);

	//Setting up Matrices
	m_shader->setUniform("ModelViewMatrix", cam.view()*m_model);	//uniform mat4 ModelViewMatrix;
	m_shader->setUniform("mvp",m);									//uniform mat4 mvp;			
	m_shader->setUniform("ViewMatrix", cam.view());				//uniform mat4 NormalMatrix;

	m_vbo->render();

	m_tex->deactivate();
	m_shader->disuse();
}