#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <gl\glew.h>
#include "Camera.h"
#include "trackball.h"
#include "PickingRay.h"


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
	bool		 isRemoved; // initially false
	int twin;		 //twin edge 
};

struct Vertex
{
	std::stack<unsigned int> e;		
};



class ObjectSlab
{
public:
	ObjectSlab(std::string filename);
   ~ObjectSlab(void);
	bool importStream(std::string filename);
	void createEdges();
	void connectTwinEdges();
	void subdivide(int idx);
	bool isSkinny(const Edge e, const glm::vec3 o);
	void flipEdge(int ei, int vi);		//flip an edge given a barycentric coordinate and an edge index
	void formFace(int ei, int vi);
	int  countActiveTriangles();
	float computeArea(int idx);		//face index is input
	void select(float mx, float my, Camera cam);
	void render(Camera cam, TrackBall* tb);
	void idle();
	void showAreas();
	void setupVertexNodes();
	
private:
	std::vector<Face> m_faces; 
	std::vector<bool> m_faceBool;	// if set to false, that face is rejected while rendering. 
	std::vector<bool> m_isSelect; //	is selected by mouse click
	std::vector<Edge> m_edges; 
	std::vector<glm::vec3> m_vertices; 
	std::vector<glm::vec3> m_normals; 
	std::vector<glm::vec3> m_texcoord; 
	std::vector<Vertex> m_VertexNode;
	glm::mat4 m_model;
	PickingRay* m_ray;
	unsigned int m_subindex;			//Temporary. Can be removed later. 
};

