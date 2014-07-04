#include "GLSLShader.h"


GLSLShader::GLSLShader(const char* vsource, const char* fsource)
{
	m_Program = glCreateProgram();
	addVertexShader(vsource);
	addFragmentShader(fsource);
	bindAttributeLocations();
	compileProgram();
}


GLSLShader::~GLSLShader(void)
{
	glDetachShader(m_Program, m_vs);
	glDetachShader(m_Program, m_fs);
	glDeleteShader(m_vs);
	glDeleteShader(m_fs);
}

void GLSLShader::bindAttributeLocations()
{
	glBindAttribLocation(m_Program, VERTEX_POSITION, "VertexPosition");
	glBindAttribLocation(m_Program, VERTEX_NORMAL, "VertexNormal");
	glBindAttribLocation(m_Program, VERTEX_TEXTURE, "VertexTexture");
	glBindAttribLocation(m_Program, VERTEX_COLOR, "VertexColor");
}

GLint GLSLShader::getUniformLocation(const GLchar *varname)
{
	return glGetUniformLocation(m_Program, varname);
}


void GLSLShader::setUniform(const GLchar* name, glm::vec3 v)
{
	GLint loc = getUniformLocation(name);
	if(loc != -1){
		glUniform3f(loc, v.x, v.y, v.z);
	}
}

void GLSLShader::setUniform(const GLchar* name, glm::vec4 v)
{
	GLint loc = getUniformLocation(name);
	if(loc != -1){
		glUniform4f(loc, v.x, v.y, v.z, v.w);
	}
}

void GLSLShader::setUniform(const GLchar* name, glm::mat3 m)
{
	GLint loc = getUniformLocation(name);
	if(loc != -1){
		glUniformMatrix3fv(loc, 1, false, glm::value_ptr(m));
	}
}

void GLSLShader::setUniform(const GLchar* name, glm::mat4 m)
{
	GLint loc = getUniformLocation(name);
	if(loc != -1){
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(m));
	}
}

void GLSLShader::addVertexShader(const char *vfile)
{
	std::string vstr(vfile);
	if(vstr.size() != 0){
		m_vs = createShader(GL_VERTEX_SHADER,file_read(vfile));
		glAttachShader(m_Program, m_vs);
		std::cout<<"Vertex Shader Added";
	}
	else{
		std::cerr<<"No Vertex Shader Detected";
	}
}

void GLSLShader::addFragmentShader(const char *ffile)
{
	std::string fstr(ffile);
	if(fstr.size() != 0){
		m_fs = createShader(GL_FRAGMENT_SHADER,file_read(ffile));
		glAttachShader(m_Program, m_fs);
		std::cout<<"Fragment Shader Added";
	}
	else{
		std::cerr<<"No Fragment Shader Detected";
	}
}


void GLSLShader::compileProgram()
{
	glLinkProgram(m_Program);

	GLint status;
	glGetProgramiv (m_Program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(m_Program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
		
}


char* GLSLShader::file_read(const char* filename)
{
	FILE* input = fopen(filename, "rb");
	if(input == NULL) return NULL;

	if(fseek(input, 0, SEEK_END) == -1) return NULL;
	long size = ftell(input);
	if(size == -1) return NULL; 
	if(fseek(input, 0, SEEK_SET) == -1) return NULL;

	char *content = (char*) malloc ((size_t)size+1);

	fread(content, 1, (size_t)size, input);
	if(ferror(input)){
		free(content);
		return NULL;
	}

	fclose(input);
	content[size] = '\0';
	return content;
}

GLuint GLSLShader::createShader(GLenum shaderType, const char* fileData)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &fileData, NULL);

	glCompileShader(shader);
	GLint status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch(shaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}