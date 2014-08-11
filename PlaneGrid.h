#pragma once

#include <GL/glew.h>
#include <glm\glm.hpp>
#include "image.h"
#include "texture.h"
#include "VertexBufferObject.h"
#include "GLSLShader.h"
#include <vector>
#include "Camera.h"
#include "trackball.h"
#include "Light.h"

struct Material
{
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
};

class PlaneGrid
{
public:
	PlaneGrid(glm::vec3 p, 
		float w, 
		float h, 
		glm::vec3 a, 
		glm::vec3 d, 
		glm::vec3 s,
		std::string textureFile, 
		std::string vertexFile, 
		std::string fragmentFile);
	~PlaneGrid(void);
	void render(Camera cam, TrackBall* tb, Light* light);
	void setMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s);
	

private:
	void init();

	float mW, mH;
	glm::vec3 mPos;
	glm::mat4 m_model;
	Texture* mTex;
	GLSLShader* m_shader; 
	VertexBufferObject* m_vbo;
	Material m_material;
};

