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
#include "PlaneGrid.h"
#include <time.h>

Shader* shader;
Pipeline* p; 
Camera cam;
TrackBall* trackBall;
Grid* terrain;
PlaneGrid* plane; 
Mesh* mesh;



int gwidth = 1024;
int gheight = 768; 
float angle = 0.f;



//Transformation Matrices
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
  glm::mat4 rot = glm::rotate(model, 45.f, glm::vec3(1, 0, 0));

  //Keyboard variables
  bool keyStates[256];

  
  int lastTime = 0;


void InitializeProgram()
{
	shader = new Shader();
	shader->add(GL_VERTEX_SHADER, "diffuseVS.glsl");
	shader->add(GL_FRAGMENT_SHADER, "diffuseFS.glsl");
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
	cam.setPosition(glm::vec3(0, 0, 20));
	cam.lookAt(glm::vec3(0.0, 0, 0.0));
	cam.setVelocity(2);
	glutWarpPointer(cam.mMouseX, cam.mMouseY);
}

void initOpengl()
{
	lastTime = glutGet(GLUT_ELAPSED_TIME);
	glEnable(GL_DEPTH_TEST);
	InitializeProgram();
	initCamera();
	trackBall = new TrackBall(gwidth, gheight);
	terrain = new Grid("textures/perlin2.jpg");
	plane = new PlaneGrid(glm::vec3(0,0,0), 512, 512);
	mesh = new Mesh(); 
	mesh->LoadMesh("models/teapot.obj");
		
}


void display()
{
	//glPolygonMode( GL_BACK, GL_LINE );
	//clear screen
	glClearColor(.25f, .25f, .25f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_ACCUM_BUFFER_BIT);

	//use shader program for subsequent calls
	shader->useProgram();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
	
	
	glm::mat4 m = cam.matrix()*trackBall->matrix()*model;
	glm::mat4 normalMatrix = glm::transpose(cam.view() * trackBall->matrix() * model);

	//Light source intensity
	glm::vec3 ld = glm::vec3(1, 1, 1);
	//diffuse reflectivity
	glm::vec3 kd = glm::vec3(1.0,1.0,1);
	
	//Pass uniforms to shader
	//Projection Matrix
	if(shader->proj_loc != -1)
		glUniformMatrix4fv(shader->proj_loc, 1, false, glm::value_ptr(cam.projection()));

	//Model View Matrix
	if(shader->mv_loc != -1)
		glUniformMatrix4fv(shader->mv_loc, 1, false, glm::value_ptr(cam.view()*model));

	//MVP Matrix
	if(shader->mvp_loc != -1)
		glUniformMatrix4fv(shader->mvp_loc, 1, false, glm::value_ptr(m));

	//Normal Matrix
	if(shader->normat_loc != -1)
		glUniformMatrix4fv(shader->normat_loc, 1, false, glm::value_ptr(normalMatrix));

	//light position
	glm::vec4 lp = cam.view() * trackBall->matrix() * model * glm::vec4(0,50, 0, 1);
	//glm::vec4 lp = glm::vec4(0,50, 0, 1);
	if(shader->lightPos_loc != -1)
		glUniform3fv(shader->lightPos_loc, 1, glm::value_ptr(lp));

	if(shader->kdloc != -1)
		glUniform3fv(shader->kdloc, 1, glm::value_ptr(kd));
	
	if(shader->ldloc != -1)
		glUniform3fv(shader->ldloc, 1, glm::value_ptr(ld));

	shader->enableShaderAttribs();
	glPointSize(2.0);
	//terrain->render(shader->positionAttrib(), shader->colorAttrib(), shader->normalAttrib());
	//plane->render(shader->positionAttrib(), shader->colorAttrib(), shader->normalAttrib(), shader->texcoordAttrib(), shader->sampleUniform());
	mesh->Render(shader->pos_loc, shader->texcoord_loc,shader->normal_loc, shader->sample_loc);

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
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		trackBall->mouseDown(x, y);
	}
	if(state == GLUT_UP && button == GLUT_LEFT_BUTTON)
	{
		trackBall->mouseUp();
	}

	if(state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)
	{
		cam.onMouseDown();
	}
	if(state == GLUT_UP && button == GLUT_RIGHT_BUTTON)
	{
		cam.onMouseUp();
	}
}

void passiveMotion(int x, int y)
{		
			
}

void motion(int x, int y)
{
	trackBall->mouseMove(x, y);
	cam.onMouseMove(x,y);	
	
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
	angle += 1.f;
	
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