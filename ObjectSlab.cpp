#include "ObjectSlab.h"

using namespace std;
using namespace glm;

ObjectSlab::ObjectSlab(std::string filename)
{
	m_model = glm::translate(glm::mat4(1), glm::vec3(0, 1, 0));
	m_model = glm::scale(m_model, glm::vec3(2, 2, 2));
	
	m_ray = new PickingRay(1024, 768);
	
	m_subindex = 0;

	importStream(filename);
	createEdges();
	connectTwinEdges();
	setupVertexNodes();
	//collapseTriangle(0);
	m_faceBool[0] = false;

}
ObjectSlab::~ObjectSlab(void)
{
}

void ObjectSlab::select(float mx, float my, Camera cam)
{
	for(int i = 6; i < 7; i++)
	{
		glm::vec3 p0 = m_vertices[m_faces[i].vi[0]];
		glm::vec3 p1 = m_vertices[m_faces[i].vi[1]];
		glm::vec3 p2 = m_vertices[m_faces[i].vi[2]];
		m_isSelect[i] = m_ray->intersect(p0, p1, p2, mx, my, cam, m_model);
	}

	if(m_subindex < m_faces.size())
	{
		cout<<"Before sub division:"<<countActiveTriangles()<<endl;
		subdivide(m_subindex);
		cout<<"After sub division:"<<countActiveTriangles()<<endl;
		m_subindex++;
	}


	
}

void ObjectSlab::render(Camera cam, TrackBall* tb)
{

	m_ray->render(cam);
	glPolygonMode(GL_FRONT, GL_FILL);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(cam.projection()));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(cam.view() * m_model));

	glBegin(GL_TRIANGLES);


	for(int i = 0 ; i < m_faces.size() ; i++ )
	{
		if(m_faceBool[i])
		{

			if(m_isSelect[i])
			{glColor3f(1.f, 0.3f, 0.f);}
			else
			{glColor3f(0.f, 0.6f, 1.f);}

			if(i%3 == 0)
				glColor3f(1.f, 0.3f, 0.f);
			if(i%3 == 1)
				glColor3f(0.f, 1.f, .6f);
			if(i%3 == 2)
				glColor3f(0.f, 0.6f, 1.f);

			float x = m_vertices[m_faces[i].vi[0]].x;
			float y = m_vertices[m_faces[i].vi[0]].y;
			float z = m_vertices[m_faces[i].vi[0]].z;
	
			glColor3f(1.f, 0.3f, 0.f);
			glVertex3f(x, y, z);
	
			x = m_vertices[m_faces[i].vi[1]].x;
			y = m_vertices[m_faces[i].vi[1]].y;
			z = m_vertices[m_faces[i].vi[1]].z;

			glColor3f(0.f, 0.6f, 1.f);
			glVertex3f(x, y, z);
	
			x = m_vertices[m_faces[i].vi[2]].x;
			y = m_vertices[m_faces[i].vi[2]].y;
			z = m_vertices[m_faces[i].vi[2]].z;

			glColor3f(0.f, 1.f, .6f);
			glVertex3f(x, y, z);
		}
	
	}
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void ObjectSlab::idle()
{
	//showAreas();
	if(m_subindex < m_faces.size())
	{
		cout<<"Before sub division:"<<countActiveTriangles()<<endl;
		if(computeArea(m_subindex) < 0.1)
		{
			collapseTriangle(m_subindex);
		}
		else{
			subdivide(m_subindex);
		}
		cout<<"After sub division:"<<countActiveTriangles()<<endl;
		m_subindex++;
	}
}

bool ObjectSlab::importStream(std::string filename)
{
	ifstream objFile(filename.c_str());

	if(objFile.fail())
	{
		std::cerr<<"Error opening File";
		return false;
	}

	std::string line; 

	while(getline(objFile, line))
	{
		if(line.size() > 1)
		{

		
		char one = line[0];
		char two = line[1];
		if((line[0] == 'f') && (line[1] = ' '))
		{
			string values = line.substr(2);
			Face face;

			//splitting into groups of three
			string g1 = values.substr(0,values.find_first_of(' '));
			string g2 = values.substr(values.find(' ')+1);
			string g3 = g2.substr(g2.find_first_of(' ')+1);
			g2 = g2.substr(0, g2.find_first_of(' '));


			size_t count = std::count(g1.begin(), g1.end(), '/');
			size_t in = g1.find("//");
			switch (count)
			{
			case 1:
				//no normal coordinates
				face.vi[0] = atoi((g1.substr(0, g1.find_first_of('/')).c_str()))-1;
				face.vi[1] = atoi((g2.substr(0, g2.find_first_of('/')).c_str()))-1;
				face.vi[2] = atoi((g3.substr(0, g3.find_first_of('/')).c_str()))-1;

				face.ti[0] = atoi((g1.substr(g1.find_first_of('/')+1, g1.size()).c_str()))-1;
				face.ti[1] = atoi((g2.substr(g2.find_first_of('/')+1, g2.size()).c_str()))-1;
				face.ti[2] = atoi((g3.substr(g3.find_first_of('/')+1, g3.size()).c_str()))-1;
				break;
			case 2:
				//is it v/t/n		or
				// v//n

				if(g1.find("//")==string::npos)
				{
					face.vi[0] = atoi((g1.substr(0, g1.find_first_of('/')).c_str()))-1;
					face.vi[1] = atoi((g2.substr(0, g2.find_first_of('/')).c_str()))-1;
					face.vi[2] = atoi((g3.substr(0, g3.find_first_of('/')).c_str()))-1;


					face.ti[0] = atoi((g1.substr(g1.find_first_of('/')+1, g1.find_last_of('/')-2)).c_str())-1;
					face.ti[1] = atoi((g2.substr(g2.find_first_of('/')+1, g2.find_last_of('/')-2).c_str()))-1;
					face.ti[2] = atoi((g3.substr(g3.find_first_of('/')+1, g3.find_last_of('/')-2).c_str()))-1;

					face.ni[0] = atoi((g1.substr(g1.find_last_of('/')+1, g1.size()).c_str()))-1;
					face.ni[1] = atoi((g2.substr(g2.find_last_of('/')+1, g2.size()).c_str()))-1;
					face.ni[2] = atoi((g3.substr(g3.find_last_of('/')+1, g3.size()).c_str()))-1;

				}
				else
				{
					face.vi[0] = atoi((g1.substr(0, g1.find_first_of('/')).c_str()))-1;
					face.vi[1] = atoi((g2.substr(0, g2.find_first_of('/')).c_str()))-1;
					face.vi[2] = atoi((g3.substr(0, g3.find_first_of('/')).c_str()))-1;

					face.ni[0] = atoi((g1.substr(g1.find_last_of('/')+1, g1.size()).c_str()))-1;
					face.ni[1] = atoi((g2.substr(g2.find_last_of('/')+1, g2.size()).c_str()))-1;
					face.ni[2] = atoi((g3.substr(g3.find_last_of('/')+1, g3.size()).c_str()))-1;

				}


				break;
			case 0:
				//no normal or texture coordinates
				face.vi[0] = atoi(g1.c_str())-1;
				face.vi[1] = atoi(g2.c_str())-1;
				face.vi[2] = atoi(g3.c_str())-1;
				break;

			}
			m_faces.push_back(face);
			m_faceBool.push_back(true);
			m_isSelect.push_back(false);
		}
		//Vertices
		if((line[0]=='v')&& (line[1] == ' '))
		{
			vec3 v;
			string values = line.substr(2);
			string g1 = values.substr(0,values.find_first_of(' '));
			string g2 = values.substr(values.find(' ')+1);
			string g3 = g2.substr(g2.find_first_of(' ')+1);
			g2 = g2.substr(0, g2.find_first_of(' '));


			v = vec3(atof(g1.c_str()), atof(g2.c_str()), atof(g3.c_str()));
			m_vertices.push_back(v);

		}

		//texture coordinates
		if((line[0]=='v')&& (line[1] == 't'))
		{
			vec3 t;
			string values = line.substr(2);
			string g1 = values.substr(0,values.find_first_of(' '));
			string g2 = values.substr(values.find(' ')+1);
			string g3 = g2.substr(g2.find_first_of(' ')+1);
			g2 = g2.substr(0, g2.find_first_of(' '));

			t = vec3(atof(g1.c_str()), atof(g2.c_str()), atof(g3.c_str()));
			m_texcoord.push_back(t);

		}
		//normal coordinates
		if((line[0]=='v')&& (line[1] == 'n'))
		{
			vec3 n;
			string values = line.substr(2);
			string g1 = values.substr(0,values.find_first_of(' '));
			string g2 = values.substr(values.find(' ')+1);
			string g3 = g2.substr(g2.find_first_of(' ')+1);
			g2 = g2.substr(0, g2.find_first_of(' '));

			n = vec3(atof(g1.c_str()), atof(g2.c_str()), atof(g3.c_str()));
			m_normals.push_back(n);
		}
		}
	}
	return true;
}

void ObjectSlab::createEdges()
{
	for(int i = 0 ; i < m_faces.size(); i++)
	{
		Edge e1;
		Edge e2;
		Edge e3;
		e1.tail = m_faces[i].vi[0];
		e1.head = m_faces[i].vi[1];
		e1.opp = m_faces[i].vi[2];
		e1.face = i;
		e1.twin = -1;
		e1.isRemoved = false; 


		e2.tail = m_faces[i].vi[1];
		e2.head = m_faces[i].vi[2];
		e2.opp = m_faces[i].vi[0];
		e2.face = i;
		e2.twin = -1;
		e2.isRemoved = false; 

		e3.tail = m_faces[i].vi[2];
		e3.head = m_faces[i].vi[0];
		e3.opp = m_faces[i].vi[1];
		e3.face = i;
		e3.twin = -1;
		e3.isRemoved = false; 

		m_faces[i].ei[0] = m_edges.size() + 0;
		m_faces[i].ei[1] = m_edges.size() + 1;
		m_faces[i].ei[2] = m_edges.size() + 2;

		m_edges.push_back(e1);
		m_edges.push_back(e2);
		m_edges.push_back(e3);
	}
}

void ObjectSlab::connectTwinEdges()
{
	for(int i = 0; i < m_edges.size(); i++)
	{
		if(!m_edges[i].isRemoved && (m_edges[i].twin == -1)) //if the edge's twin is not set, go ahead and set it
		{
			for(int j = 0; j < m_edges.size(); j++)
			{
				if((m_edges[i].head == m_edges[j].tail) && (m_edges[i].tail == m_edges[j].head))
				{
					m_edges[i].twin = j;
					m_edges[j].twin = i;
				}
			}
		}
	}
}

void ObjectSlab::subdivide(int idx)
{
	if(m_faceBool[idx])		//do not subdivide a face which is already subdivided and removed
	{
		Face f = m_faces[idx];
		m_faceBool[idx] = false; 
		
		vec3 A = m_vertices[f.vi[0]];
		vec3 B = m_vertices[f.vi[1]];
		vec3 C = m_vertices[f.vi[2]];

		vec3 O = (A+B+C) / 3.f;

		vec3 AB = A - B; 
		vec3 BC = B - C; 
		vec3 CA = C - A;
	
		vec3 nor = glm::cross(AB, BC);
		nor = normalize(nor);
		nor = nor * .2f; 
	
		//O = nor + O;

		//new vertex is pushed
		m_vertices.push_back(O);

		//index of new vertex is size - 1
		int idxO = m_vertices.size() - 1;

		Vertex v; 
		v.v = idxO;
		m_VertexNode.push_back(v);
		
		if(isSkinny(m_edges[f.ei[0]], m_vertices[idxO])){
			flipEdge(f.ei[0], idxO);
		} else{
			formFace(f.ei[0], idxO);
		}

		if(isSkinny(m_edges[f.ei[1]], m_vertices[idxO])){
			flipEdge(f.ei[1], idxO);
		} else{
			formFace(f.ei[1], idxO);
		}

		if(isSkinny(m_edges[f.ei[2]], m_vertices[idxO])){
			flipEdge(f.ei[2], idxO);
		} else {
			formFace(f.ei[2], idxO);
		}

		//remove edges of the face; 
		m_edges[m_faces[idx].ei[0]].isRemoved = true;
		m_edges[m_faces[idx].ei[1]].isRemoved = true;
		m_edges[m_faces[idx].ei[2]].isRemoved = true;

		connectTwinEdges();


	}
}

bool ObjectSlab::isSkinny(const Edge e, const glm::vec3 o)
{
	float threshold = 20; 

	vec3 a = m_vertices[e.head];
	vec3 b = m_vertices[e.tail];

	//compute the edges
	vec3 ab = a - b;
	vec3 oa = o - a;
	vec3 bo = b - o;
	

	float cos1 = abs(dot(ab,oa) / (length(ab)*length(oa))) ;
	float cos2 = abs(dot(oa,bo) / (length(oa)*length(bo))) ;
	float cos3 = abs(dot(bo,ab) / (length(bo)*length(ab))) ;

	float angle1 = acos(cos1) * 180.f / M_PI;
	float angle2 = acos(cos2) * 180.f / M_PI;
	float angle3 = acos(cos3) * 180.f / M_PI;

	//if any of the angles are less than the threshold angle then the triangle is skinny
	if((angle1 < threshold)||(angle2 < threshold)||(angle3 < threshold))
		return true;

	return false; 
}
//flip edges clockwise
/*
Flip Edge 
input: edge index and
	   and index of new vertex which 
	   subdivides it's parent face into three faces

output: flips the edge clockwise
*/
void ObjectSlab::flipEdge(int ei, int vi)
{
	Edge e = m_edges[ei];
	vec3 O = m_vertices[vi]; //barycenter computed previously

	//face of e is already deleted before the flip edge call
	//delete the e's twin's face ( or rather stop rendering it)
	m_faceBool[m_edges[e.twin].face]  = false;
	//delete edges of the twin face
	m_edges[m_faces[m_edges[e.twin].face].ei[0]].isRemoved = true;
	m_edges[m_faces[m_edges[e.twin].face].ei[1]].isRemoved = true;
	m_edges[m_faces[m_edges[e.twin].face].ei[2]].isRemoved = true;


	int oppo = m_edges[e.twin].opp;		//opposite vertex of the twin

	Face f3; //new face
	Face f4; //new face

	f3.vi[0] = oppo; 
	f3.vi[1] = e.head;
	f3.vi[2] = vi;

	f4.vi[0] = vi; 
	f4.vi[1] = e.tail; 
	f4.vi[2] = oppo;

	Edge e1; //for face f3
	Edge e2; //for face f3
	Edge e3; //for face f3

	Edge e4; //for face f4
	Edge e5; //for face f4
	Edge e6; //for face f4

	//Edges for face f3
	e1.tail = f3.vi[0];
	e1.head = f3.vi[1];
	e1.opp = f3.vi[2];
	e1.face = m_faces.size();
	e1.twin = -1;
	e1.isRemoved = false;	

	e2.tail = f3.vi[1];
	e2.head = f3.vi[2];
	e2.opp = f3.vi[0];
	e2.face = m_faces.size();
	e2.twin = -1;
	e2.isRemoved = false;


	e3.tail = f3.vi[2];
	e3.head = f3.vi[0];
	e3.opp = f3.vi[1];
	e3.face = m_faces.size();
	e3.twin = -1;
	e3.isRemoved = false;
	//For the new barycenter vertex, assign this edge
	m_VertexNode[f3.vi[2]].e.push_back(m_edges.size() + 2);

	//Edges for face f4
	e4.tail = f4.vi[0];
	e4.head = f4.vi[1];
	e4.opp = f4.vi[2];
	e4.face = m_faces.size()+1;
	e4.twin = -1;
	e4.isRemoved = false;
	m_VertexNode[f4.vi[0]].e.push_back(m_edges.size() + 3);

	e5.tail = f4.vi[1];
	e5.head = f4.vi[2];
	e5.opp = f4.vi[0];
	e5.face = m_faces.size()+1;
	e5.twin = -1;
	e5.isRemoved = false;

	e6.tail = f4.vi[2];
	e6.head = f4.vi[0];
	e6.opp = f4.vi[1];
	e6.face = m_faces.size()+1;
	e6.twin = -1;
	e6.isRemoved = false;

	//connect faces with edges
	f3.ei[0] = m_edges.size() + 0;
	f3.ei[1] = m_edges.size() + 1;
	f3.ei[2] = m_edges.size() + 2;

	f4.ei[0] = m_edges.size() + 3;
	f4.ei[1] = m_edges.size() + 4;
	f4.ei[2] = m_edges.size() + 5;

	m_edges.push_back(e1);
	m_edges.push_back(e2);
	m_edges.push_back(e3);
	m_edges.push_back(e4);
	m_edges.push_back(e5);
	m_edges.push_back(e6);

	m_faces.push_back(f3);
	m_faces.push_back(f4);
	m_faceBool.push_back(true); // f3
	m_faceBool.push_back(true); // f4 
	m_isSelect.push_back(false); // f3
	m_isSelect.push_back(false); // f4
}

void ObjectSlab::formFace(int ei, int idxO)
{
	Face f1;
	f1.vi[0] = m_edges[ei].tail;
	f1.vi[1] = m_edges[ei].head;
	f1.vi[2] = idxO;

	Edge e1; 
	Edge e2; 
	Edge e3; 

	//Edges for face f3
	e1.tail = f1.vi[0];
	e1.head = f1.vi[1];
	e1.opp = f1.vi[2];
	e1.face = m_faces.size();
	e1.twin = -1;
	e1.isRemoved = false;

	e2.tail = f1.vi[1];
	e2.head = f1.vi[2];
	e2.opp = f1.vi[0];
	e2.face = m_faces.size();
	e2.twin = -1;
	e2.isRemoved = false;

	e3.tail = f1.vi[2];
	e3.head = f1.vi[0];
	e3.opp = f1.vi[1];
	e3.face = m_faces.size();
	e3.twin = -1;
	e3.isRemoved = false;

	m_VertexNode[f1.vi[2]].e.push_back(m_edges.size() + 2);

	f1.ei[0] = m_edges.size() + 0;
	f1.ei[1] = m_edges.size() + 1;
	f1.ei[2] = m_edges.size() + 2;

	m_edges.push_back(e1);
	m_edges.push_back(e2);
	m_edges.push_back(e3);

	m_faces.push_back(f1);
	m_faceBool.push_back(true);
	m_isSelect.push_back(false); 
}

int ObjectSlab::countActiveTriangles()
{
	int count = 0;
	if(m_faces.size() == m_faceBool.size())
	{
		for(int i = 0 ; i < m_faces.size(); i ++)
		{
			if(m_faceBool[i])
				count++;
		}
	}
	return count;
}

float ObjectSlab::computeArea(int idx)
{
	Face f = m_faces[idx];

	vec3 a = m_vertices[f.vi[0]];
	vec3 b = m_vertices[f.vi[1]];
	vec3 c = m_vertices[f.vi[2]];

	vec3 ab = a - b;
	vec3 ac = a - c;

	float area = fabs(0.5*glm::length(cross(ab, ac)));

	return area;
}

void ObjectSlab::showAreas()
{
	std::vector<float> areas;
	std::vector<float> lol;
	for(int i = 0 ; i < m_faces.size() ; i++)
	{
		areas.push_back(computeArea(i));
	}
	lol = areas;
}

void ObjectSlab::setupVertexNodes()
{
	for(int i = 0 ; i < m_vertices.size() ; i ++)
	{
		Vertex v;
		v.v = i;
		for(int j = 0 ; j < m_edges.size() ; j++)
		{
			if(m_edges[j].tail == i)
			{
				v.e.push_back(j);
			}
		}
		m_VertexNode.push_back(v);
	}
}

void ObjectSlab::collapseTriangle(int idx)
{
	//Get reference to Face index
	Face f = m_faces[idx];
	m_faceBool[idx] = false;

	//get reference to all edges
	Edge e1 = m_edges[f.ei[0]];
	Edge e2 = m_edges[f.ei[1]];
	Edge e3 = m_edges[f.ei[2]];


	vec3 A = m_vertices[f.vi[0]];
	vec3 B = m_vertices[f.vi[1]];
	vec3 C = m_vertices[f.vi[2]];

	vec3 O = (A+B+C) / 3.f;

	m_vertices.push_back(O);

	//index of new vertex is size - 1
	int idxO = m_vertices.size() - 1;
	Vertex v;
	v.v = idxO;
	m_VertexNode.push_back(v);

	//collapse to point
	collapseToPoint(f.ei[0], idxO);
	collapseToPoint(f.ei[1], idxO);
	collapseToPoint(f.ei[2], idxO);


	//mark each edge as deleted
	m_edges[m_faces[idx].ei[0]].isRemoved = true;
	m_edges[m_faces[idx].ei[1]].isRemoved = true;
	m_edges[m_faces[idx].ei[2]].isRemoved = true;

	connectTwinEdges();
}

void ObjectSlab::collapseToPoint(int ei, int vi)
{
	//Get reference to adjacent face
	Face f = m_faces[m_edges[m_edges[ei].twin].face];

	//alter the edges dependent on ei's end points
	Edge twin = m_edges[m_edges[ei].twin];
	Vertex v1 = m_VertexNode[twin.head];
	Vertex v2 = m_VertexNode[twin.tail];

	for(int i = 0 ; i < v1.e.size(); i++)
	{
		m_edges[v1.e[i]].tail = vi;
		m_edges[m_edges[v1.e[i]].twin].head = vi;
		m_VertexNode[vi].e.push_back(v1.e[i]);
	}

	for(int i = 0 ; i < v2.e.size(); i++)
	{
		m_edges[v2.e[i]].tail = vi;
		m_edges[m_edges[v2.e[i]].twin].head = vi;
		m_VertexNode[vi].e.push_back(v2.e[i]);
	}


	//Delete this face
	m_faceBool[m_edges[m_edges[ei].twin].face] = false;

	m_edges[f.ei[0]].isRemoved = true;
	m_edges[f.ei[1]].isRemoved = true;
	m_edges[f.ei[2]].isRemoved = true;
}