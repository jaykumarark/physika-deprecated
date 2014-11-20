#include "ObjectSlab.h"

using namespace std;
using namespace glm;

ObjectSlab::ObjectSlab(std::string filename)
{
	m_model = glm::translate(glm::mat4(1), glm::vec3(0, 1, 0));
	m_model = glm::scale(m_model, glm::vec3(10, 10, 10));
	
	m_ray = new PickingRay(1024, 768);
	
	m_subindex = 0;

	importStream(filename);
	connectTwinEdges();
	setupVertexNodes();

	mToggleDeletedFaces = false;
}

ObjectSlab::~ObjectSlab(void)
{
}

void ObjectSlab::select(float mx, float my, ACamera* cam)
{
	int faceIndex;
	for(int i = 0; i < m_faces.size(); i++)
	{
		if(m_faceBool[i]){
			Edge e[3];
			e[0] = m_edges[m_faces[i].ei];
			e[1] = m_edges[e[0].next];
			e[2] = m_edges[e[1].next];

			vec3 p0 = m_vertices[e[0].tail];
			vec3 p1 = m_vertices[e[1].tail];
			vec3 p2 = m_vertices[e[2].tail];
		
			if(!m_isSelect[i]){
				//m_isSelect[i] = m_ray->intersect(p0, p1, p2, mx, my, cam, m_model);
			}
			faceIndex = i;
		}
	}	

	collapseTriangle(faceIndex);
}

void ObjectSlab::render(ACamera* cam, TrackBall* tb)
{

	//m_ray->render(cam);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(cam->projection()));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(cam->view() * m_model));

	glBegin(GL_TRIANGLES);


	for(int i = 0 ; i < m_faces.size() ; i++ )
	{
		bool render;
		if (mToggleDeletedFaces)
		{
			render = !m_faceBool[i];
		} else {
			render = m_faceBool[i];
		}
		
		if(render)
		{
			Face f = m_faces[i];
			Edge e1 = m_edges[f.ei];

			float x = m_vertices[e1.tail].x;
			float y = m_vertices[e1.tail].y;
			float z = m_vertices[e1.tail].z;
	
			
			if(m_isSelect[i]){
				glColor3f(1.f, 0.3f, 0.f);
			} else{glColor3f(1.f, 0.3f, 0.f);}
			
			glVertex3f(x, y, z);
	
			x = m_vertices[e1.head].x;
			y = m_vertices[e1.head].y;
			z = m_vertices[e1.head].z;

			if(m_isSelect[i]){
				glColor3f(1.f, 0.3f, 0.f);
			} else{glColor3f(0.f, 0.6f, 1.f);}
			
			glVertex3f(x, y, z);
	
			x = m_vertices[m_edges[e1.next].head].x;
			y = m_vertices[m_edges[e1.next].head].y;
			z = m_vertices[m_edges[e1.next].head].z;

			if(m_isSelect[i]){
				glColor3f(1.f, 0.3f, 0.f);
			} else{glColor3f(0.f, 1.f, .6f);}
			
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
		if(!isFaceBoundary(m_subindex))
			collapseTriangle(m_subindex);
		//subdivide(m_subindex);
	//	/*if(computeArea(m_subindex) < 0.1)
	//	{
	//		collapseTriangle(m_subindex);
	//	}
	//	else{
	//		subdivide(m_subindex);
	//	}*/
		m_subindex++;
	}
}

bool ObjectSlab::isFaceBoundary(int idx)
{
	Face f = m_faces[idx];
	Edge e1, e2, e3; 
	e1 = m_edges[f.ei];
	e2 = m_edges[e1.next];
	e3 = m_edges[e2.next];

	
	if(e1.twin == -1)
		return true;
	if(e2.twin == -1)
		return true;
	if(e3.twin == -1)
		return true;

	return false;
}

std::string ObjectSlab::trim(std::string const& str)
{
	std::size_t first = str.find_first_not_of(' ');
	std::size_t last  = str.find_last_not_of(' ');
	return str.substr(first, last-first+1);
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
				string values = trim(line.substr(2));

				Face face;
				Edge e1;
				Edge e2; 
				Edge e3;
				int v[3], t[3], n[3];

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
					v[0] = atoi((g1.substr(0, g1.find_first_of('/')).c_str()))-1;
					v[1] = atoi((g2.substr(0, g2.find_first_of('/')).c_str()))-1;
					v[2] = atoi((g3.substr(0, g3.find_first_of('/')).c_str()))-1;

					t[0] = atoi((g1.substr(g1.find_first_of('/')+1, g1.size()).c_str()))-1;
					t[1] = atoi((g2.substr(g2.find_first_of('/')+1, g2.size()).c_str()))-1;
					t[2] = atoi((g3.substr(g3.find_first_of('/')+1, g3.size()).c_str()))-1;

					e1.tail = v[0]; 
					e1.head = v[1];
					e1.next = m_edges.size()+1;
					e1.twin = -1;
					e1.isRemoved = false;
					e1.face = m_faces.size()+0;

					e2.tail = v[1];
					e2.head = v[2];
					e2.next = m_edges.size()+2;
					e2.twin = -1;
					e2.isRemoved = false;
					e2.face = m_faces.size()+0;

					e3.tail = v[2]; 
					e3.head = v[0];
					e3.next = m_edges.size()+0;
					e3.twin = -1;
					e3.isRemoved = false;
					e3.face = m_faces.size()+0;

					face.ei = m_edges.size() + 0;

					break;
				case 2:
					//is it v/t/n		or
					// v//n

					

					if(g1.find("//")==string::npos)
					{
						v[0] = atoi((g1.substr(0, g1.find_first_of('/')).c_str()))-1;
						v[1] = atoi((g2.substr(0, g2.find_first_of('/')).c_str()))-1;
						v[2] = atoi((g3.substr(0, g3.find_first_of('/')).c_str()))-1;

						t[0] = atoi((g1.substr(g1.find_first_of('/')+1, g1.find_last_of('/')-2)).c_str())-1;
						t[1] = atoi((g2.substr(g2.find_first_of('/')+1, g2.find_last_of('/')-2).c_str()))-1;
						t[2] = atoi((g3.substr(g3.find_first_of('/')+1, g3.find_last_of('/')-2).c_str()))-1;

						n[0] = atoi((g1.substr(g1.find_last_of('/')+1, g1.size()).c_str()))-1;
						n[1] = atoi((g2.substr(g2.find_last_of('/')+1, g2.size()).c_str()))-1;
						n[2] = atoi((g3.substr(g3.find_last_of('/')+1, g3.size()).c_str()))-1;

						e1.tail = v[0]; 
						e1.head = v[1];
						e1.next = m_edges.size()+1;
						e1.twin = -1;
						e1.face = m_faces.size()+0;
						e1.isRemoved = false;

						e2.tail = v[1];
						e2.head = v[2];
						e2.next = m_edges.size()+2;
						e2.twin = -1;
						e2.face = m_faces.size()+0;
						e2.isRemoved = false;

						e3.tail = v[2]; 
						e3.head = v[0];
						e3.next = m_edges.size()+0;
						e3.twin = -1;
						e3.face = m_faces.size()+0;
						e3.isRemoved = false;

						face.ei = m_edges.size() + 0;
					}
					else
					{
						v[0] = atoi((g1.substr(0, g1.find_first_of('/')).c_str()))-1;
						v[1] = atoi((g2.substr(0, g2.find_first_of('/')).c_str()))-1;
						v[2] = atoi((g3.substr(0, g3.find_first_of('/')).c_str()))-1;

						n[0] = atoi((g1.substr(g1.find_last_of('/')+1, g1.size()).c_str()))-1;
						n[1] = atoi((g2.substr(g2.find_last_of('/')+1, g2.size()).c_str()))-1;
						n[2] = atoi((g3.substr(g3.find_last_of('/')+1, g3.size()).c_str()))-1;

						e1.tail = v[0]; 
						e1.head = v[1];
						e1.next = m_edges.size()+1;
						e1.twin = -1;
						e1.face = m_faces.size()+0;
						e1.isRemoved = false;

						e2.tail = v[1];
						e2.head = v[2];
						e2.next = m_edges.size()+2;
						e2.twin = -1;
						e2.face = m_faces.size()+0;
						e2.isRemoved = false;

						e3.tail = v[2]; 
						e3.head = v[0];
						e3.next = m_edges.size()+0;
						e3.twin = -1;
						e3.face = m_faces.size()+0;
						e3.isRemoved = false;

						face.ei = m_edges.size() + 0;
					}


					break;
				case 0:
					//no normal or texture coordinates
					v[0] = atoi(g1.c_str())-1;
					v[1] = atoi(g2.c_str())-1;
					v[2] = atoi(g3.c_str())-1;

					e1.tail = v[0]; 
					e1.head = v[1];
					e1.next = m_edges.size()+1;
					e1.face = m_faces.size()+0;
					e1.twin = -1;
					e1.isRemoved = false;

					e2.tail = v[1];
					e2.head = v[2];
					e2.next = m_edges.size()+2;
					e2.face = m_faces.size()+0;
					e2.twin = -1;
					e2.isRemoved = false;

					e3.tail = v[2]; 
					e3.head = v[0];
					e3.next = m_edges.size()+0;
					e3.face = m_faces.size()+0;
					e3.twin = -1;
					e3.isRemoved = false;

					face.ei = m_edges.size() + 0;

					break;

				}
				m_edges.push_back(e1);
				m_edges.push_back(e2);
				m_edges.push_back(e3);
				m_faces.push_back(face);
				m_faceBool.push_back(true);
				m_isSelect.push_back(false);
			}
			//Vertices
			if((line[0]=='v')&& (line[1] == ' '))
			{
				vec3 v;
				string values = trim(line.substr(2));
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
				string values = trim(line.substr(2));
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
				string values = trim(line.substr(2));
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

void ObjectSlab::connectTwinEdges()
{
	for(int i = 0; i < m_edges.size(); i++)
	{
		if(!m_edges[i].isRemoved && (m_edges[i].twin == -1)) //if the edge's twin is not set, go ahead and set it
		{
			findATwin(i);
		}
	}
}

void ObjectSlab::findATwin(int ei)
{
	bool found = false;
	for(int j = 0 ; j < m_edges.size(); j++)
	{
		if((m_edges[ei].head == m_edges[j].tail) && (m_edges[ei].tail == m_edges[j].head))
		{
			m_edges[ei].twin = j;
			m_edges[j].twin = ei;
			found = true;
			break;
		}
	}

	if(!found)
	{
		cout<<"Something went wrong with twin search"<<endl;
	}
}

void ObjectSlab::subdivide(int idx)
{
	if(m_faceBool[idx])		//do not subdivide a face which is already subdivided and removed
	{
		Face f = m_faces[idx];
		m_faceBool[idx] = false; 

		int e1 = f.ei;
		int e2 = m_edges[e1].next;
		int e3 = m_edges[e2].next;
		//remove edges of the face; 
		m_edges[e1].isRemoved = true;
		m_edges[e2].isRemoved = true;
		m_edges[e3].isRemoved = true;


		
		vec3 A = m_vertices[m_edges[e1].tail];
		vec3 B = m_vertices[m_edges[e1].head];
		vec3 C = m_vertices[m_edges[e2].head];

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
		
		if(isSkinny(m_edges[e1], m_vertices[idxO])){
			flipEdge(e1, idxO);
		} else{
			formFace(e1, idxO);
		}

		if(isSkinny(m_edges[e2], m_vertices[idxO])){
			flipEdge(e2, idxO);
		} else{
			formFace(e2, idxO);
		}

		if(isSkinny(m_edges[e3], m_vertices[idxO])){
			flipEdge(e3, idxO);
		} else {
			formFace(e3, idxO);
		}

		connectTwinEdges();


	}
}

bool ObjectSlab::isSkinny(const Edge &e, const glm::vec3 &o)
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

	Edge ee = m_edges[e.twin];

	//delete edges of the twin face
	m_edges[e.twin].isRemoved = true;
	m_edges[ee.next].isRemoved = true;
	m_edges[m_edges[ee.next].next].isRemoved = true;

	int oppo = m_edges[m_edges[e.twin].next].head;		//opposite vertex of the twin

	Face f3; //new face
	Face f4; //new face

	int v1[3], v2[3];

	v1[0] = oppo; 
	v1[1] = e.head;
	v1[2] = vi;

	v2[0] = vi; 
	v2[1] = e.tail; 
	v2[2] = oppo;

	Edge e1; //for face f3
	Edge e2; //for face f3
	Edge e3; //for face f3

	Edge e4; //for face f4
	Edge e5; //for face f4
	Edge e6; //for face f4

	//Edges for face f3
	e1.tail = v1[0]; 
	e1.head = v1[1];
	e1.next = m_edges.size()+1;
	e1.face = m_faces.size()+0;
	e1.twin = -1;
	e1.isRemoved = false;

	e2.tail = v1[1];
	e2.head = v1[2];
	e2.next= m_edges.size()+2;
	e2.face = m_faces.size()+0;
	e2.twin = -1;
	e2.isRemoved = false;


	e3.tail = v1[2];
	e3.head = v1[0];
	e3.next = m_edges.size()+0;
	e3.face = m_faces.size()+0;
	e3.twin = -1;
	e3.isRemoved = false;

	//For the new barycenter vertex, assign this edge
	m_VertexNode[v1[2]].e.push_back(m_edges.size() + 2);

	//Edges for face f4
	e4.tail = v2[0];
	e4.head = v2[1];
	e4.next = m_edges.size()+4;
	e4.face = m_faces.size()+1;
	e4.twin = -1;
	e4.isRemoved = false;
	m_VertexNode[v2[0]].e.push_back(m_edges.size() + 3);

	e5.tail = v2[1];
	e5.head = v2[2];
	e5.next = m_edges.size()+5;
	e5.face = m_faces.size()+1;
	e5.twin = -1;
	e5.isRemoved = false;

	e6.tail = v2[2];
	e6.head = v2[0];
	e6.next = m_edges.size()+3;
	e6.face = m_faces.size()+1;
	e6.twin = -1;
	e6.isRemoved = false;

	//connect faces with one edge

	f3.ei = m_edges.size() + 0;
	f4.ei = m_edges.size() + 3;
	

	m_edges.push_back(e1);
	m_edges.push_back(e2);
	m_edges.push_back(e3);
	m_edges.push_back(e4);
	m_edges.push_back(e5);
	m_edges.push_back(e6);

	/*findATwin(m_edges.size()-6);
	findATwin(m_edges.size()-5);
	findATwin(m_edges.size()-4);
	findATwin(m_edges.size()-3);
	findATwin(m_edges.size()-2);
	findATwin(m_edges.size()-1);*/

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
	int v[3];
	v[0] = m_edges[ei].tail;
	v[1] = m_edges[ei].head;
	v[2] = idxO;

	Edge e1; 
	Edge e2; 
	Edge e3; 

	//Edges for face f3
	e1.tail = v[0];
	e1.head = v[1];
	e1.next = m_edges.size()+1;
	e1.face = m_faces.size();
	e1.twin = -1;
	e1.isRemoved = false;

	e2.tail = v[1];
	e2.head = v[2];
	e2.next = m_edges.size()+2;
	e2.face = m_faces.size();
	e2.twin = -1;
	e2.isRemoved = false;

	e3.tail = v[2];
	e3.head = v[0];
	e3.next = m_edges.size()+0;
	e3.face = m_faces.size();
	e3.twin = -1;
	e3.isRemoved = false;

	m_VertexNode[v[2]].e.push_back(m_edges.size() + 2);

	f1.ei = m_edges.size() + 0;
	
	m_edges.push_back(e1);
	m_edges.push_back(e2);
	m_edges.push_back(e3);

	/*findATwin(m_edges.size()-3);
	findATwin(m_edges.size()-2);
	findATwin(m_edges.size()-1);
	*/
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
	Edge e = m_edges[f.ei];

	vec3 a = m_vertices[e.tail];
	vec3 b = m_vertices[e.head];
	vec3 c = m_vertices[m_edges[e.next].head];

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
			if(m_edges[j].tail == i && !m_edges[j].isRemoved)
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
	int e1 = f.ei;
	int e2 = m_edges[e1].next;
	int e3 = m_edges[e2].next;

	//mark each edge as deleted
	m_edges[e1].isRemoved = true;
	m_edges[e2].isRemoved = true;
	m_edges[e3].isRemoved = true;

	vec3 A = m_vertices[m_edges[e1].tail];
	vec3 B = m_vertices[m_edges[e2].tail];
	vec3 C = m_vertices[m_edges[e3].tail];

	vec3 O = (A+B+C) / 3.f;

	m_vertices.push_back(O);

	//index of new vertex is size - 1
	int idxO = m_vertices.size() - 1;
	Vertex v;
	v.v = idxO;
	m_VertexNode.push_back(v);

	//collapse to point
	collapseToPoint(e1, idxO);
	collapseToPoint(e2, idxO);
	collapseToPoint(e3, idxO);

	connectTwinEdges();
	
}

void ObjectSlab::collapseToPoint(int ei, int vi)
{
	//Get reference to adjacent face
	Face f = m_faces[m_edges[m_edges[ei].twin].face];
	int e[3];

	e[0] = m_edges[ei].twin;
	e[1] = m_edges[e[0]].next;
	e[2] = m_edges[e[1]].next;

	int twin1 = m_edges[e[1]].twin;
	int twin2 = m_edges[e[2]].twin;

	//Delete this face
	m_faceBool[m_edges[m_edges[ei].twin].face] = false;

	m_edges[e[0]].isRemoved = true;
	m_edges[e[1]].isRemoved = true;
	m_edges[e[2]].isRemoved = true;

	//alter the edges dependent on ei's end points
	Edge twin = m_edges[m_edges[ei].twin];
	Vertex v1 = m_VertexNode[twin.head];
	Vertex v2 = m_VertexNode[twin.tail];

	for(int i = 0 ; i < v1.e.size(); i++)
	{
		if(!m_edges[v1.e[i]].isRemoved)
		{
			m_edges[v1.e[i]].tail = vi;
		}
		if(!m_edges[m_edges[v1.e[i]].twin].isRemoved){
			m_edges[m_edges[v1.e[i]].twin].head = vi;
		}
		
	}

	for(int i = 0 ; i < v2.e.size(); i++)
	{
		if(!m_edges[v2.e[i]].isRemoved)
		{
			m_edges[v2.e[i]].tail = vi;
		}
		if(!m_edges[m_edges[v2.e[i]].twin].isRemoved){
			m_edges[m_edges[v2.e[i]].twin].head = vi;
		}
	}
	//findATwin(twin1);
	//findATwin(twin2);
}