#include "PlaneGrid.h"


PlaneGrid::PlaneGrid(glm::vec3 p, 
					 float w, 
					 float h, 
					 glm::vec3 a, 
					 glm::vec3 d, 
					 glm::vec3 s, 
					 std::string textureFile, 
					 std::string vertexFile, 
					 std::string fragmentFile)
{
	mPos = p;
	mW = w;
	mH = h;
	mTex = new Texture(GL_TEXTURE_2D, textureFile, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	m_vbo = new VertexBufferObject(GL_ARRAY_BUFFER, GL_TRIANGLES);
	m_shader = new GLSLShader(vertexFile.c_str(), fragmentFile.c_str());
	m_model = glm::translate(glm::mat4(1.f), glm::vec3(0, - 20, 0));
	init();
	setMaterial(a, d, s);
}

PlaneGrid::~PlaneGrid(void)
{
	delete m_vbo;
	delete m_shader;
}

void PlaneGrid::setMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s)
{
	m_material.Ka = a; 
	m_material.Kd = d; 
	m_material.Ks = s; 
}

void PlaneGrid::init()
{
	vector<glm::vec3> verts; 
	vector<glm::vec2> tcoords;
	vector<unsigned int> indices; 
	vector<glm::vec3> normals;
	vector<ModelLoader::Vertex> data;

	//row-wise grid arrangement
	glm::vec3 p1 = glm::vec3(mPos.x-(mW/2), 0, mPos.z-(mH/2));
	glm::vec3 p2 = glm::vec3(mPos.x+(mW/2), 0, mPos.z-(mH/2));
	glm::vec3 p3 = glm::vec3(mPos.x-(mW/2), 0, mPos.z+(mH/2));
	glm::vec3 p4 = glm::vec3(mPos.x+(mW/2), 0, mPos.z+(mH/2));

	glm::vec2 t1 = 6.f * glm::vec2(p1.x/mW, p1.z/mH);
	glm::vec2 t2 = 6.f * glm::vec2(p2.x/mW, p2.z/mH);
	glm::vec2 t3 = 6.f * glm::vec2(p3.x/mW, p3.z/mH);
	glm::vec2 t4 = 6.f * glm::vec2(p4.x/mW, p4.z/mH);
	
	verts.push_back(p1);
	verts.push_back(p2);
	verts.push_back(p3);
	verts.push_back(p4);

	tcoords.push_back(t1);
	tcoords.push_back(t2);
	tcoords.push_back(t3);
	tcoords.push_back(t4);

	//push indices
	//triangle 1
	indices.push_back(0);		//1
	indices.push_back(2);		//6
	indices.push_back(1);		//2

	//triangle 2
	indices.push_back(1);		//2
	indices.push_back(2);		//6
	indices.push_back(3);		//7

	for(int i = 0; i < verts.size(); i++)
	{
		normals.push_back(glm::vec3(0,0,0));
	}

	//This works for row wise grids 
	//Make sure the grid is row wise else the normals are going to be incorrect. 
	for(int i = 0; i < indices.size();i+=3)
	{
		unsigned int idx1 = indices[i];
		unsigned int idx2 = indices[i+1];
		unsigned int idx3 = indices[i+2];

		glm::vec3 A = verts[idx1];
		glm::vec3 B = verts[idx2];
		glm::vec3 C = verts[idx3];

		glm::vec3 AB = B-A; 
		glm::vec3 AC = C-A; 
		glm::vec3 cross = glm::cross(AB, AC);

		normals[idx1] += cross;
		normals[idx2] += cross;
		normals[idx3] += cross;

	}

	for(int i = 0 ; i < verts.size();i++)
	{
		normals[i] = glm::normalize(normals[i]);
	}

	for(int i = 0 ; i < verts.size(); i++)
	{
		ModelLoader::Vertex d; 
		d.p = verts[i];
		d.t = tcoords[i];
		d.n = normals[i];
		data.push_back(d);
	}

	m_vbo->init(data, indices);
}


void PlaneGrid::render(Camera cam, TrackBall* tb, Light* light)
{
	glPolygonMode(GL_FRONT, GL_FILL);

	//Light Position
	Light::LightProperties lprops = light->properties();
	glm::vec4 lp = glm::vec4(light->position(), 1.f);

	m_shader->use();

	//setup textures
	mTex->activate(GL_TEXTURE0);
	m_shader->setSampler("TextureSample2D", 0);

	//Setting up Matrices
	glm::mat4 m = cam.matrix() * tb->matrix() * m_model;
	glm::mat4 normalMatrix = glm::transpose(cam.view() * tb->matrix() * m_model);

	m_shader->setUniform("ProjectionMatrix", cam.projection());		//uniform mat4 ProjectionMatrix; 
	m_shader->setUniform("ModelViewMatrix",  m_model);	//uniform mat4 ModelViewMatrix;
	m_shader->setUniform("mvp",m);									//uniform mat4 mvp;			
	m_shader->setUniform("NormalMatrix", normalMatrix);	

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

	mTex->deactivate();
	m_shader->disuse();
}


