#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <math.h>
#include "Camera.h"
#include "TrackBall.h"
#include "pipeline.h"
#include "cube.h"
#include "image.h"
#include "mesh.h"
#include "grid.h"
#include <time.h>

#define IDX(i,j,col) j+col*i 

Shader* shader;
Pipeline* p; 
Camera cam;
TrackBall* trackBall;

Grid* terrain;

int gwidth = 1024;
int gheight = 768; 
float angle;

bool isMouseDown = false; 


//Transformation Matrices
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
  glm::mat4 rot = glm::rotate(model, 45.f, glm::vec3(1, 0, 0));

  //Keyboard variables
  bool keyStates[256];

  
  int lastTime = 0;


void InitializeProgram()
{
	shader = new Shader();
	shader->add(GL_VERTEX_SHADER, "vs.glsl");
	shader->add(GL_FRAGMENT_SHADER, "fs.glsl");
	shader->CompileProgram();
	shader->deleteShaders();
	shader->initShaderVars();
}

void initCamera(){
	cam.setWindowCoords(gwidth, gheight);
	cam.init();
	cam.setFov(60);
	cam.setAspRatio(gwidth/gheight);
	cam.setNearFar(0.1f, 3000.f);
	cam.setPosition(glm::vec3(50, 100, 50));
	cam.lookAt(glm::vec3(100.0, 100, 0.0));
	cam.setVelocity(50);
	glutWarpPointer(cam.mMouseX, cam.mMouseY);
}


void initOpengl()
{
	isMouseDown = false; 
	lastTime = glutGet(GLUT_ELAPSED_TIME);
	glEnable(GL_DEPTH_TEST);
	InitializeProgram();
	initCamera();
	trackBall = new TrackBall(gwidth, gheight);
	terrain = new Grid("textures/perlin2.jpg");
		
}


void display()
{
	glPolygonMode( GL_BACK, GL_LINE );
	//clear screen
	glClearColor(0.11f, 0.11f, 0.11f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glAccum(GL_RETURN, 0.95f);

	glClear(GL_ACCUM_BUFFER_BIT);

	//use shader program for subsequent calls
	shader->useProgram();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
	
	glm::mat4 m = cam.matrix() *trackBall->transform()*model;
	
	//Pass uniforms to shader
	if(shader->matrixUniform() != -1)
		glUniformMatrix4fv(shader->matrixUniform(), 1, false, glm::value_ptr(m));

	shader->enableShaderAttribs();
	
	

	terrain->render(shader->positionAttrib(), shader->colorAttrib(), shader->normalAttrib());

	glUseProgram(0);
	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape (int w, int h)
{
	gwidth = w;
	gheight = h; 

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	  case 27:
		  glutLeaveMainLoop();
		  return;
	}
	keyStates[key] = true;
}

void keyboardUp(unsigned char key, int x, int y)
{
	keyStates[key] = false;
}

void mwheel(int wheel, int direction, int x, int y)
{
	
}

void mouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		trackBall->setBtnState(true);
		trackBall->trackPoint(x, y);
		isMouseDown = true;
	}
	if(state == GLUT_UP)
	{
		trackBall->setBtnState(false);
		isMouseDown = true;
	}

}

void passiveMotion(int x, int y)
{		
	
		
}

void motion(int x, int y)
{
	cam.onMouseMove(x,y);	
	//trackBall->trackMovement(x, y);
}



void processKeyboard(float dt)
{
	
	
	if(keyStates['a'] || keyStates['A'])
	{
		cam.offsetPosition(cam.velocity()*dt*-cam.right());
	}

	if(keyStates['s'] || keyStates['S'])
	{
		cam.offsetPosition(cam.velocity()*dt*-cam.forward());
	}

	if(keyStates['w'] || keyStates['W'])
	{
		cam.offsetPosition(cam.velocity()*dt*cam.forward());
	}

	if(keyStates['d'] || keyStates['D'])
	{
		cam.offsetPosition(cam.velocity()*dt*cam.right());
	}

	if(keyStates['j'] || keyStates['J'])
	{
		rot = glm::rotate(rot,-1.f, glm::vec3(0.0, 1.0, 0.0));
	}

	if(keyStates['l'] || keyStates['L'])
	{
		rot = glm::rotate(rot ,1.f, glm::vec3(0.0, 1.0, 0.0));
	}
	if(keyStates['k'] || keyStates['k'])
	{
		rot = glm::rotate(rot,-1.f, glm::vec3(1.0, 0.0, 0.0));
	}

	if(keyStates['i'] || keyStates['I'])
	{
		rot = glm::rotate(rot ,1.f, glm::vec3(1.0, 0.0, 0.0));
	}
	
}

void idle()
{
	float presentTime = glutGet(GLUT_ELAPSED_TIME);
	float dt= 0.1;
	angle += 0.2;
	
	processKeyboard(dt);
	lastTime = presentTime;
}

void createGlutCallBacks()
{
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutReshapeFunc(reshape);
	glutMouseWheelFunc(mwheel);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(passiveMotion);
	glutMotionFunc(motion);
	glutIdleFunc(idle);
	
	
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ACCUM);
	glutInitWindowSize(gwidth,gheight);
	glutInitWindowPosition(100, 200);
	glutCreateWindow("Physika - Terrain Generator");
	createGlutCallBacks();
	GLenum res = glewInit();
	if(res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	initOpengl();
	glutMainLoop();
	return 0;
}