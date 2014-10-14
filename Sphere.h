#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm\glm.hpp>
#include "camera.h"
#include "VertexBufferObject.h"
#include "GLSLShader.h"
#include "Texture.h"
#include "Light.h"
#include <vector>

#define M_PI 3.1415926535897932384626433832795

class Sphere
{
public:

	struct Material
	{
		glm::vec3 Ka;
		glm::vec3 Kd;
		glm::vec3 Ks;
	};


	Sphere(glm::vec3 p,int stacks, int slices, float radius, std::string vertexFile, std::string fragmentFile,
			glm::vec3 a, glm::vec3 d, glm::vec3 s);
	void render(Camera cam, Light* light);
private: 

	void init();
	void initGrid();
	void setMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s);
	void initBuffers();

	int mStacks, mSlices;
	float mRadius;
	glm::vec3 m_pos; 
	glm::mat4 m_model;
	GLSLShader* m_shader;
	vector<glm::vec3> mVertices;
	vector<unsigned int> mIndices;
	Texture* mTexture;
	VertexBufferObject* m_vbo;
	Material m_material;

};
