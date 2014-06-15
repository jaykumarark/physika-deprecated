#include "trackball.h"


TrackBall::TrackBall(int width, int height)
{
	mWidth = width; 
	mHeight = height;
	rotateStep = 50.f;
	isDown = false;
	m_rotate = glm::mat4(1);
	lastPoint = glm::vec3(0,0,0);
	currPoint = glm::vec3(0,0,0);
}
TrackBall::~TrackBall(void){}

void TrackBall::mouseDown(int x, int y)
{
	isDown = true;
	lastPoint = twoDto3D(x, y);
}

void TrackBall::mouseMove(int x, int y)
{
	if(isDown)
	{
		currPoint = twoDto3D(x,y);

		glm::vec3 dir = currPoint - lastPoint;
		float vel = mag(dir);
		if(vel > 0.0001)
		{
			glm::vec3 axis = glm::cross(currPoint, lastPoint);
			float angle = vel * 90;
			print(currPoint);
			cout<<"vel: "<<vel;
			lastPoint = currPoint;
			m_rotate = glm::rotate(glm::mat4(1),angle, axis);
		}
	}
}

void TrackBall::mouseUp()
{
	isDown = false;
}

float TrackBall::mag(glm::vec3 v)
{
	return sqrt((v.x*v.x)+(v.y*v.y)+(v.z*v.z));
}

glm::vec3 TrackBall::twoDto3D(int x, int y)
{
	glm::vec3 v = glm::vec3();
	v.x = (2*(float)x - float(mWidth)) / float(mWidth);
	v.y = (float(mHeight)- 2*(float)y)/float(mHeight);
	float d = sqrt((v.x*v.x)+(v.y*v.y));
	d = (d<1.0f)?d:1.0f;
	v.z = sqrt(1-(d*d));
	v = glm::normalize(v);
	return v;
}

glm::mat4 TrackBall::matrix()
{
	return m_rotate;
}


void TrackBall::print(glm::vec3 v)
{
	cout<<"x: "<<v.x<<endl;
	cout<<"y: "<<v.y<<endl;
	cout<<"z: "<<v.z<<endl;
}