
#ifndef SKYBOX_H
#define SKYBOX_H

#include "GLSLShader.h"
#include "VertexBufferObject.h"
#include "ModelLoader.h"
#include "Camera.h"
#include "trackball.h"
#include "Light.h"
#include "CubeMap.h"

class Skybox
{
public:
	Skybox(glm::vec3 p,
		std::string modelFile, 
		vector<std::string> textureFile, 
		std::string vertexFile, 
		std::string fragmentFile);

	void init();
	void render(Camera cam);
	~Skybox(void);

private:
	GLSLShader* m_shader;
	VertexBufferObject* m_vbo;
	ModelLoader* m_objLoader; 
	glm::mat4 m_model; 
	CubeMap* m_tex;
	std::string m_vertexFile;
	std::string m_fragmentFile;
};

#endif