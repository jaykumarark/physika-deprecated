#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm\glm.hpp>
#include "image.h"
#include "Camera.h"
#include "trackball.h"
#include "Light.h"
#include "VertexBufferObject.h"
#include "GLSLShader.h"
#include "image.h"
#include "texture.h"
#include "ModelLoader.h"

class NewGrid
{
public:

	struct Material
	{
		glm::vec3 Ka;
		glm::vec3 Kd;
		glm::vec3 Ks;
	};

	/*
	 for terrains with height maps already presented
	 use the image width and height as dimension for the 
	 terrain
	*/

	NewGrid(glm::vec3 pos, 
			int cs,
			int elevation,
			std::string textureFile, 
			std::string heightMapFile, 
			std::string vertexFile, 
			std::string fragmentFile,
			glm::vec3 a, 
			glm::vec3 d,
			glm::vec3 s);

	NewGrid(glm::vec3 pos, 
			int w, 
			int h, 
			int cs,
			int elevation,
			std::string textureFile, 
			std::string vertexFile, 
			std::string fragmentFile,
			glm::vec3 a, 
			glm::vec3 d,
			glm::vec3 s);

	void render(Camera cam, TrackBall* tb, Light* light);
	void setMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s);
	~NewGrid(void);
private:
	void init();

	int mW;					//total width along x-axis
	int mH;					//total height along y-axis
	int m_cs;				//side of a cell (square)
	float m_elevation;		//Max Height of terrain in pixels
	glm::vec3 m_pos;		//centre of the grid
	glm::mat4 m_model; 
	Image* m_Heightmap;
	Texture* mTex; 
	GLSLShader* m_shader; 
	VertexBufferObject* m_vbo; 
	Material m_material; 

};

