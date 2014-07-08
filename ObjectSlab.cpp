#include "ObjectSlab.h"

using namespace std;
using namespace glm;

ObjectSlab::ObjectSlab(std::string filename)
{
	m_model = glm::translate(glm::mat4(1), glm::vec3(0, 1, 0));
	m_model = glm::scale(m_model, glm::vec3(5, 5, 5));
	
	m_ray = new PickingRay(1024, 768);
	
	m_subindex = 0;

	importStream(filename);
	createEdges();
	connectTwinEdges();
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
		subdivide(m_subindex);
		m_subindex++;
	}

	//subdivide(6);
	
	

	
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

			if(!m_faceBool[i])
			{glColor3f(1.f, 0.3f, 0.f);}
			else
			{glColor3f(0.f, 0.6f, 1.f);}


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


void ObjectSlab::importStream(std::string filename)
{
	ifstream objFile(filename.c_str());

	if(objFile.fail())
	{
		std::cerr<<"Error opening File";
	}

	std::string line; 

	while(getline(objFile, line))
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


		e2.tail = m_faces[i].vi[1];
		e2.head = m_faces[i].vi[2];
		e2.opp = m_faces[i].vi[0];
		e2.face = i;
		e2.twin = -1;

		e3.tail = m_faces[i].vi[2];
		e3.head = m_faces[i].vi[0];
		e3.opp = m_faces[i].vi[1];
		e3.face = i;
		e3.twin = -1;

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
		if(m_edges[i].twin == -1)
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


	flipEdge(f.ei[0], idxO);
	flipEdge(f.ei[1], idxO);
	flipEdge(f.ei[2], idxO);

	
	/*Face f1;
	Face f2;
	Face f3;


	f1.vi[0] = f.vi[0];
	f1.vi[1] = f.vi[1];
	f1.vi[2] = idxO;

	f2.vi[0] = idxO;
	f2.vi[1] = f.vi[1];
	f2.vi[2] = f.vi[2];

	f3.vi[0] = f.vi[2];
	f3.vi[1] = f.vi[0];
	f3.vi[2] = idxO;

	//m_faces.push_back(f1);
	m_faces.push_back(f2);
	m_faces.push_back(f3);

	//m_faceBool.push_back(true);
	m_faceBool.push_back(true);
	m_faceBool.push_back(true);*/
}

bool ObjectSlab::isSkinny(Edge e, glm::vec3 o)
{
	return true; 
}

//flip edges clockwise
void ObjectSlab::flipEdge(int ei, int vi)
{
	Edge e = m_edges[ei];
	vec3 O = m_vertices[vi]; //barycenter computed previously

	//face of e is already deleted before the flip edge call
	//delete the e's twin's face ( or rather stop rendering it)
	m_faceBool[m_edges[e.twin].face]  = false;

	int oppo = m_edges[e.twin].opp;		//opposite vertex of the twin

	Face f3; //new face
	Face f4; //new face

	f3.vi[0] = oppo; 
	f3.vi[1] = e.head;
	f3.vi[2] = vi;

	f4.vi[0] = vi; 
	f4.vi[1] = e.tail; 
	f4.vi[2] = oppo;

	m_faces.push_back(f3);
	m_faces.push_back(f4);

	m_faceBool.push_back(true); // f3
	m_faceBool.push_back(true); // f4 
}