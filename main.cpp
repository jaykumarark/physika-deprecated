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
#include "PlaneGrid.h"
#include "PObject.h"`
#include "Light.h"
#include "Grid.h"
#include "Skybox.h"
#include <time.h>
#include "PAudio.h"
#include "Sphere.h"
#include "ObjectSlab.h"

int gwidth = 1024;
int gheight = 768;

Camera cam;
TrackBall* trackBall;
PlaneGrid* plane; 
PObject* teapot;
Light* sceneLight; 


Grid* terrain;
PAudio* audio;
Sphere* sphere;
ObjectSlab* slab;


//CubeMap


 
//Keyboard variables
bool keyStates[256];


void initCamera(){
	cam.setWindowCoords(gwidth, gheight);
	cam.init();
	cam.setFov(60);
	cam.setAspRatio(gwidth/gheight);
	cam.setNearFar(1.f, 3000.f);
	cam.setPosition(glm::vec3(0, 20, -20));
	cam.lookAt(glm::vec3(0, 0, 0.0));
	cam.setVelocity(20);
	glutWarpPointer(cam.mMouseX, cam.mMouseY);
}

void initOpengl()
{
	glEnable(GL_DEPTH_TEST);
	initCamera();
	trackBall			= new TrackBall(gwidth, gheight);
	plane				= new PlaneGrid(glm::vec3(-128,0,128),
										2, 128, 
										"textures/floor.png",
										"gridVS.glsl", 
										"gridFS.glsl",
										glm::vec3(0.2), 
										glm::vec3(1.), 
										glm::vec3(0.0));
		
	//terrain					= new Grid(glm::vec3(-128,0,128), 
	//									 2, 
	//									 50.f, 
	//									"textures/perlin2.jpg",
	//									"textures/grass.jpg",
	//									"terrainVS.glsl", 
	//									"terrainFS.glsl", 
	//									glm::vec3(1), 
	//									glm::vec3(1), 
	//									glm::vec3(1));

	sphere				= new Sphere(glm::vec3(-20,15,0),20, 20, 4, "vs.glsl", 
												"fs.glsl",
												glm::vec3(1), 
												glm::vec3(1), 
												glm::vec3(1.0));



	teapot				= new PObject(glm::vec3(0,10,0),"models/box.obj",
									  "textures/grass.jpg",
									  "diffuseVS.glsl",
									  "diffuseFS.glsl",
									  glm::vec3(1), 
									  glm::vec3(0.4,1.f,0.6), 
									  glm::vec3(0.5));

	sceneLight			= new Light(20,
									glm::vec3(0, 40, 0), 
									glm::vec3(0.2, 0.2, 0.2), 
									glm::vec3(0.7),
									glm::vec3(1));

	slab = new ObjectSlab("models/box.obj");
	//audio				= new PAudio("iphone_metrognome_remix.mp3");
	//audio->play();
}


void display()
{
	//clear screen
	glClearColor(.15f, .15f, .15f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//teapot->render(cam, trackBall, sceneLight); 
	//plane->render(cam, trackBall, sceneLight);
	//terrain->render(cam, trackBall, sceneLight);
	//sceneLight->render(cam, trackBall);

	//sphere->render(cam, sceneLight);

	slab->render(cam, trackBall);

	//sky->render(cam);

	/*glColor3f(1.f, 0.f, 0.f);
	glPointSize(5.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(cam.projection()));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(cam.view()));

	float *spec = audio->spectrum();
	glBegin(GL_POINTS);
	for(int i = 3 ; i < 63 ; i+=2 )
	{
	glVertex3f(i, spec[i] * 100 , 0);
	glVertex3f((i+1), spec[i] * 75 , 0);
	}
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/


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
	if(state == GLUT_DOWN && button == GLUT_MIDDLE_BUTTON)
	{
		//triangle->select(x, y, cam);
	}

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
	
	if(keyStates['p'] || keyStates['P'])
	{
		slab->idle();
	}
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
	if(keyStates['v'] || keyStates['V'])
	{
		float *spec = audio->spectrum();
		for(int i = 0 ; i < 64 ; i++)
		{
			cout<<i<<": "<<spec[i]<<endl;
		}
	}
	
}

void idle()
{
	float dt= 0.1;
	processKeyboard(dt);
	if(keyStates['P'] || keyStates['p'])
	{
		//triangle->idle();
	}
	sceneLight->update();
	//audio->update();
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ACCUM |GL_MULTISAMPLE);
	glutInitWindowSize(gwidth,gheight);
	glutInitWindowPosition(100, 30);
	glutCreateWindow("Physika");
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