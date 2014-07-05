#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <gl\glew.h>
#include "Camera.h"
#include "trackball.h"


struct Face
{
	unsigned int vi[3];	// ccw winding of vertex indices
	unsigned int ti[3];	// texture indices
	unsigned int ni[3]; // normal indices
	unsigned int ei[3]; // index of the edges
};

struct Edge
{
	//indices of the vertices of an edge
	unsigned int head; 
	unsigned int tail; 
	unsigned int opp; //vertex opposite to an edge in a face
	unsigned int face; //reference to a face
	int twin;		 //twin edge 
};

class ObjectSlab
{
public:
	ObjectSlab(std::string filename);
	void importStream(std::string filename);
	void createEdges();
	void connectTwinEdges();
	void render(Camera cam, TrackBall* tb);
	~ObjectSlab(void);

private:
	std::vector<Face> m_faces; 
	std::vector<bool> m_faceBool;	// if set to false, that face is rejected while rendering. 
	std::vector<Edge> m_edges; 
	std::vector<glm::vec3> m_vertices; 
	std::vector<glm::vec3> m_normals; 
	std::vector<glm::vec3> m_texcoord; 
	glm::mat4 m_model;
};

