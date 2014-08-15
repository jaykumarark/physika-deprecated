#pragma once

#include <glm\glm.hpp>
#include <vector>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

class ModelLoader
{
public:

	struct Vertex
	{
		glm::vec3 p; 
		glm::vec2 t; 
		glm::vec3 n;
		glm::vec3 c;

		Vertex() {}

		Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal, const glm::vec3& color)
		{
			p    = pos;
			t    = tex;
			n	 = normal;
			c	 = color;
		}
	};

	ModelLoader(const std::string filename);
	~ModelLoader(void);
	bool LoadMesh(const std::string filename);
	std::vector<Vertex> data();
	std::vector<unsigned int> indices();

private: 
	void InitFromScene(const aiScene* pScene, const std::string& filename);
	void InitMesh(const aiMesh* paiMesh);
	std::vector<Vertex> m_data; 
	std::vector<unsigned int> m_indices;
	bool m_hasTextures;
};

