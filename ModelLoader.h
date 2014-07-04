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
		glm::vec3 m_pos; 
		glm::vec2 m_tex; 
		glm::vec3 m_normal;

		Vertex() {}

		Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
		{
			m_pos    = pos;
			m_tex    = tex;
			m_normal = normal;
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

