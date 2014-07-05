#include "PlaneGrid.h"


PlaneGrid::PlaneGrid(glm::vec3 p, float w, float h)
{
	mPos = p;
	mW = w;
	mH = h;
	//mTex = new Texture(GL_TEXTURE_2D, "textures/gridtexture128.jpg");
	m_vbo = new VertexBufferObject(GL_ARRAY_BUFFER, GL_TRIANGLES);
	m_shader = new GLSLShader("gridVS.glsl", "gridFS.glsl");
	m_model = glm::mat4(1.f);
	init();
}

PlaneGrid::~PlaneGrid(void)
{
	delete m_vbo;
	delete m_shader;
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

	glm::vec2 t1 = glm::vec2(p1.x/mW, p1.z/mH);
	glm::vec2 t2 = glm::vec2(p2.x/mW, p2.z/mH);
	glm::vec2 t3 = glm::vec2(p3.x/mW, p3.z/mH);
	glm::vec2 t4 = glm::vec2(p4.x/mW, p4.z/mH);
	
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


void PlaneGrid::render(Camera cam, TrackBall* tb)
{
	//mTex->activate(sample, GL_TEXTURE0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_shader->use();

	glm::mat4 m = cam.matrix() * tb->matrix() * m_model;
	glm::mat4 normalMatrix = glm::transpose(cam.view() * tb->matrix() * m_model);

	//Setting up Matrices
	m_shader->setUniform("ProjectionMatrix", cam.projection());		//uniform mat4 ProjectionMatrix; 
	m_shader->setUniform("ModelViewMatrix", cam.view() * m_model);	//uniform mat4 ModelViewMatrix;
	m_shader->setUniform("mvp",m);									//uniform mat4 mvp;			
	m_shader->setUniform("NormalMatrix", normalMatrix);	

	//Light Position
	glm::vec4 lp = cam.view() * m_model * glm::vec4(0,50,0,1);
	//glm::vec4 lp = glm::vec4(0,50,0,1);
	m_shader->setUniform("lightPosition", lp);
	//Material 
	glm::vec3 kd = glm::vec3(1.f, 1.f, 1.f);
	m_shader->setUniform("kd", kd);
	//Light intensity
	glm::vec3 ld = glm::vec3(1.f, 1.f, 1.f);
	m_shader->setUniform("ld", ld);

	m_vbo->render();

	m_shader->disuse();
}


