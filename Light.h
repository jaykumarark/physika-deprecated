#pragma once
#include <glm\glm.hpp>
#include "VertexBufferObject.h"
#include "GLSLShader.h"
#include  "ModelLoader.h"
#include "ACamera.h"
#include "trackball.h"

class Light
{
public:

	struct LightProperties {
		glm::vec3 Ia;	//ambient	
		glm::vec3 Id;	//diffuse
		glm::vec3 Is;	//specular
	};


	Light(float radius, glm::vec3 c, glm::vec3 a, glm::vec3 d, glm::vec3 s);
	~Light(void);
	void init(); 
	void render(ACamera* cam);
	glm::vec3 position();
	LightProperties properties(); 
	void setProperty(glm::vec3 a, glm::vec3 d, glm::vec3 s);
	void update();
	

private:
	VertexBufferObject* m_vbo; 
	GLSLShader* m_shader; 
	ModelLoader* m_objLoader; 
	glm::vec3 m_pos; 
	glm::vec3 m_Center;
	glm::mat4 m_model;
	LightProperties m_props; 
	float m_angle;
	float mR;

};

