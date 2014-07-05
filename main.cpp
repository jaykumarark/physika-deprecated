#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <math.h>
#include "Camera.h"
#include "TrackBall.h"
#include "cube.h"
#include "PlaneGrid.h"
#include "PObject.h"
#include "ObjectSlab.h"
#include "PickingRay.h"
#include <time.h>

int gwidth = 1024;
int gheight = 768;

Camera cam;
TrackBall* trackBall;
PlaneGrid* plane; 
ObjectSlab* triangle; 
PObject* teapot;
PickingRay* pr; 
 
//Keyboard variables
bool keyStates[256];


void initCamera(){
	cam.setWindowCoords(gwidth, gheight);
	cam.init();
	cam.setFov(60);
	cam.setAspRatio(gwidth/gheight);
	cam.setNearFar(0.1f, 3000.f);
	cam.setPosition(glm::vec3(0, 20, 20));
	cam.lookAt(glm::vec3(0.0, 20, 0.0));
	cam.setVelocity(2);
	glutWarpPointer(cam.mMouseX, cam.mMouseY);
}

void initOpengl()
{
	glEnable(GL_DEPTH_TEST);
	initCamera();
	trackBall			= new TrackBall(gwidth, gheight);
	plane				= new PlaneGrid(glm::vec3(0,0,0), 64, 64);	
	teapot				= new PObject("models/teapot.obj");
	triangle			= new ObjectSlab("models/bunny.obj");
	pr					= new PickingRay(gwidth, gheight);
}


void display()
{
	//clear screen
	glClearColor(.15f, .15f, .15f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//teapot->render(cam, trackBall); 
	plane->render(cam, trackBall);
	triangle->render(cam, trackBall);
	pr->render(cam);
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
	
	pr->getRay(x, y, cam);
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
		cam.offsetPosition(cam.velocity()*dt*-cam.up());
	}

	if(keyStates['w'] || keyStates['W'])
	{
		cam.offsetPosition(cam.velocity()*dt*cam.up());
	}

	if(keyStates['d'] || keyStates['D'])
	{
		cam.offsetPosition(cam.velocity()*dt*cam.right());
	}
	
}

void idle()
{
	float dt= 0.1;
	processKeyboard(dt);
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