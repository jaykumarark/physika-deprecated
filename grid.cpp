#include "grid.h"


Grid::Grid(glm::vec3 pos, 
		   int cs,
		   int elevation,
		   std::string heightMapFile,
		   std::string textureFile,
		   std::string vertexFile, 
		   std::string fragmentFile,
		   glm::vec3 a, 
		   glm::vec3 d,
		   glm::vec3 s)
{
	m_pos = pos;
	m_cs = cs;
	m_elevation = elevation;
	m_Heightmap = new Image(heightMapFile, true);
	mW = m_Heightmap->width(); 
	mH= m_Heightmap->height(); 
	mTex = new Texture(GL_TEXTURE_2D, textureFile, GL_REPEAT, GL_LINEAR, GL_LINEAR);
	m_vbo = new VertexBufferObject(GL_ARRAY_BUFFER, GL_TRIANGLES);
	m_shader = new GLSLShader(vertexFile.c_str(), fragmentFile.c_str());
	m_model = glm::translate(glm::mat4(1), pos);
	setMaterial(a, d, s);
	init();
}

Grid::~Grid(void)
{
	delete mTex; 
	delete m_shader; 
	delete m_vbo;
}


void Grid::setMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s)
{
	m_material.Ka = a; 
	m_material.Kd = d; 
	m_material.Ks = s; 
}

void Grid::init()
{
	vector<glm::vec3> verts; 
	vector<glm::vec2> tcoords;
	vector<unsigned int> indices; 
	vector<glm::vec3> normals;
	vector<glm::vec3> colors; 
	vector<ModelLoader::Vertex> data;

	/*int gridX = m_Heightmap->width(); 
	int gridY = m_Heightmap->height();
	int gSize = 20;*/
	for(int j = 0; j < mH; j++)
	{
		for(int i = 0; i < mW; i++)
		{
			glm::vec3 v  = glm::vec3(i*m_cs, -m_elevation*m_Heightmap->getColorAt(i, j), -j*m_cs);
			glm::vec3 c = glm::vec3 ((float)i/255,  m_Heightmap->getColorAt(i, j), (float)j/128);
			glm::vec2 t = 4.f * glm::vec2(v.x/mW, v.z/mH);
			colors.push_back(c);
			verts.push_back(v);
			tcoords.push_back(t);
		}
	}

	//create indices for the grid
	for( int j = 0; j < mH-1 ; j++)
	{

		for( int i = 0; i < mW-1; i++)
		{
			int v1 = i + j*mW;
			int v2 = (i + 1) + j*mW;
			int v6 = i+ (j+1)*mW;
			int v7 = (i+1)+((j+1)*mW);


			indices.push_back(v1);
			indices.push_back(v6);
			indices.push_back(v2);
			
			indices.push_back(v2);
			indices.push_back(v6);
			indices.push_back(v7);
			
		}
	}


	//initialize m_normals
	for(int i = 0 ; i < verts.size();i++)
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
		glm::vec3 cross = glm::cross(AC, AB);

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
		d.c = colors[i];
		data.push_back(d);
	}

	m_vbo->init(data, indices);
}


void Grid::render(Camera cam, TrackBall* tb, Light* light)
{
	//Light Position
	Light::LightProperties lprops = light->properties();
	glm::vec4 lp = glm::vec4(light->position(), 1.f);


	m_shader->use();

	//setup textures
	mTex->activate(GL_TEXTURE0);
	m_shader->setSampler("TextureSample2D", 0);

	//Setting up Matrices
	glm::mat4 m = cam.matrix() * tb->matrix() * m_model;
	//glm::mat4 normalMatrix = glm::transpose(cam.view() * tb->matrix() * m_model);

	m_shader->setUniform("ProjectionMatrix", cam.projection());		//uniform mat4 ProjectionMatrix; 
	m_shader->setUniform("ModelViewMatrix",  cam.view()*m_model);	//uniform mat4 ModelViewMatrix;
	m_shader->setUniform("mvp",m);									//uniform mat4 mvp;			
	m_shader->setUniform("ViewMatrix", cam.view());	

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

