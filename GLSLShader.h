#pragma once



#define VERTEX_POSITION		0
#define VERTEX_NORMAL		1
#define VERTEX_TEXTURE		2
#define VERTEX_COLOR		3

#include <gl/glew.h>
#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLSLShader
{
public:
	GLSLShader(const char* vsource, const char* fsource);
	~GLSLShader(void);
	void use();   //use shader program
	void disuse();	//disable shader program
	void setUniform(const GLchar* name, glm::mat3 m);
	void setUniform(const GLchar* name, glm::mat4 m);
	void setUniform(const GLchar* name, glm::vec3);
	void setUniform(const GLchar* name, glm::vec4);

private:
	char* file_read(const char* filename);
	GLuint createShader(GLenum shaderType, const char* fileData);
	void addVertexShader(const char* vfile);
	void addFragmentShader(const char* ffile);
	GLint getUniformLocation(const GLchar *varname);
	void compileProgram();
	void bindAttributeLocations();
	GLuint m_Program;
	GLuint m_vs;
	GLuint m_fs;
	
};

