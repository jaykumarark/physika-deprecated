#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\norm.hpp>
#include <gl\glew.h>
#include "Camera.h"
#include "trackball.h"
#include "PickingRay.h"
#include "ACamera.h"


struct Face
{
	unsigned int ei;	//refernce one edge preferrably first one
};

struct Edge
{
	//indices of the vertices of an edge
	unsigned int head; 
	unsigned int tail; 
	unsigned int next; //next edge index;
	unsigned int face; //reference to a face
	int			 twin; //twin edge 
	bool		 isRemoved; // initially false
	
};

struct Vertex
{
	unsigned int v;	 //indices of m_vertices
	std::vector<int> e;	//indices of m_edge
};



class ObjectSlab
{
public:
	ObjectSlab(std::string filename);
   ~ObjectSlab(void);
	std::string trim(std::string const &str);
	bool importStream(std::string filename);
	void connectTwinEdges();
	void subdivide(int idx);
	bool isSkinny(const Edge &e, const glm::vec3 &o);
	void flipEdge(int ei, int vi);		//flip an edge given a barycentric coordinate and an edge index
	void formFace(int ei, int vi);
	int  countActiveTriangles();
	float computeArea(int idx);		//face index is input
	void select(float mx, float my, ACamera* cam);
	void render(ACamera* cam, TrackBall* tb);
	void idle();
	void showAreas();
	void setupVertexNodes();
	void collapseTriangle(int idx);	//face index is input
	void findATwin(int ei);
	void collapseToPoint(int ei);
	bool mToggleDeletedFaces;
	bool isFaceBoundary(int idx);
	float edgeLength(int ei);
	bool is_collapse_ok(int ei);
	bool intersect(glm::vec3, glm::vec3, glm::vec3, glm::vec3);
	
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

