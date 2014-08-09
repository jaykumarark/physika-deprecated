#pragma once

#include "GLSLShader.h"
#include "VertexBufferObject.h"
#include "ModelLoader.h"
#include "Camera.h"
#include "trackball.h"
#include "Light.h"
#include "texture.h"


class PObject
{
public:
	struct Material
	{
		glm::vec3 Ka;
		glm::vec3 Kd; 
		glm::vec3 Ks; 
	};


	PObject(std::string modelFile, 
			std::string textureFile, 
			std::string vertexFile, 
			std::string fragmentFile,
			glm::vec3 a, 
			glm::vec3 d, 
			glm::vec3 s);
	void init();
	void render(Camera cam, TrackBall* tb, Light* light);
	void setMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s);
	~PObject(void);

private:
	GLSLShader* m_shader;
	VertexBufferObject* m_vbo;
	ModelLoader* m_objLoader;
	glm::mat4 m_model;		//model matrix is specific to each object and not global
	Material m_material;
	Texture* m_tex;
	std::string m_vertexFile;
	std::string m_fragmentFile;
};

