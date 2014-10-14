#include "Sphere.h"


Sphere::Sphere(glm::vec3 p,int stacks, int slices, float radius, std::string vertexFile, std::string fragmentFile,
			   glm::vec3 a, glm::vec3 d, glm::vec3 s)
{
	mStacks = stacks;
	mSlices = slices;
	mRadius = radius;
	m_vbo = new VertexBufferObject(GL_ARRAY_BUFFER, GL_TRIANGLES);
	m_shader = new GLSLShader(vertexFile.c_str(), fragmentFile.c_str());
	m_model = glm::translate(glm::mat4(1), p);
	mTexture= new Texture(GL_TEXTURE_2D, "textures/grass.jpg", GL_CLAMP, GL_LINEAR, GL_LINEAR);
	init();
	setMaterial(a, d, s);
}
void Sphere::setMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s)
{
	m_material.Ka = a; 
	m_material.Kd = d; 
	m_material.Ks = s; 
}
void Sphere::init()
{
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices; 
	std::vector<glm::vec2> tcoords;
	std::vector<glm::vec3> colors;
	std::vector<ModelLoader::Vertex> data;

	float dtheta = 2 * M_PI / mSlices;	// Horizontal 
	float dphi = M_PI / mStacks ;		// vertical 
	float r = mRadius;
	for(int i = 0; i < mStacks+1; i++)
	{
		for(int j = 0; j < mSlices; j++)
		{
			float x = r * cos(j*dtheta) * sin(i*dphi);
			float y = r * cos(i*dphi);
			float z = r * sin(j*dtheta) * sin(i*dphi);
			glm::vec3 v = glm::vec3(x, y, z);
			glm::vec3 n = glm::normalize(v);
			float u = 0.5 + (atan2(n.z, n.x)/(2*M_PI));
			float vv = 0.5 - (asin(n.y)/M_PI);
			verts.push_back(v);
			normals.push_back(n);
			tcoords.push_back(glm::vec2(u,vv));
			colors.push_back(glm::vec3(i*1.f, j*1.f, 1));
		}
	}

	for( int j = 0; j < mStacks ; j++)
	{

		for( int i = 0; i < mSlices; i++)
		{
			int v1 = i + j*mSlices;
			int v2 = (i + 1) + j*mSlices;
			int v6 = i+ (j+1)*mSlices;
			int v7 = (i+1)+((j+1)*mSlices);


			indices.push_back(v1);
			indices.push_back(v6);
			indices.push_back(v2);

			indices.push_back(v2);
			indices.push_back(v6);
			indices.push_back(v7);

		}
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

void Sphere::render(Camera cam, Light* light)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Light Position
	Light::LightProperties lprops = light->properties();
	glm::vec4 lp = glm::vec4(light->position(), 1.f);

	m_shader->use();
	mTexture->activate(GL_TEXTURE0);
	m_shader->setSampler("TextureSample2D", 0);

	//Setting up Matrices
	glm::mat4 m = cam.matrix() * m_model;
	glm::mat3 NormalMatrix = glm::mat3(cam.view()*m_model);
	NormalMatrix = glm::transpose(glm::inverse(NormalMatrix));

	m_shader->setUniform("ProjectionMatrix", cam.projection());		//uniform mat4 ProjectionMatrix; 
	m_shader->setUniform("ModelViewMatrix",  cam.view()*m_model);	//uniform mat4 ModelViewMatrix;
	m_shader->setUniform("mvp",m);									//uniform mat4 mvp;			
	m_shader->setUniform("ViewMatrix", cam.view());	
	m_shader->setUniform("NormalMatrix", NormalMatrix);

	m_shader->setUniform("EyePositionInWorld", cam.position());

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

	mTexture->deactivate();
	m_shader->disuse();	



	
}