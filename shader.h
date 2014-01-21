#include<vector>
#include<gl/glew.h>
/*
Shader Class
	-> Load shaders from file
	-> Attach shader to program
	-> Compile program and link
	-> Check for error messages in the shader

*/

using namespace std;

class Shader{

private:
	vector<GLuint> shaderList;
	GLuint Program;

	char* file_read(const char* filename);
	GLuint CreateShader(GLenum shaderType, const char* strShaderFileData);
	GLuint pos_loc; 
	GLuint sample_loc;
	GLuint normal_loc;
	GLuint texcoord_loc;
	GLuint color_loc;
	GLuint matrix_loc;



public:
	Shader();
	void enableShaderAttribs();
	void disableShaderAttribs();
	void initShaderVars();
	void add(GLenum shaderType, const char* filename);
	void deleteShaders();
	void CompileProgram();
	void useProgram();
	GLint getUniformLoc(GLchar* varName);
	GLint getAttributeLocation(std::string varName);
	int positionAttrib();
	int normalAttrib();
	int texcoordAttrib();
	int colorAttrib();
	int sampleUniform();
	int matrixUniform();



};