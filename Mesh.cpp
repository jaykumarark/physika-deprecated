#include "Mesh.h"

bool Mesh::LoadMesh(const std::string& Filename)
{
	//Release previously loaded mesh
	Clear();

	bool Ret = false; 
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if(pScene)
	{
		Ret = InitFromScene(pScene, Filename);
	}
	else
	{
		 printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}
	return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	//init meshes in scene
	for(unsigned int i = 0; i < m_Entries.size() ; i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return InitMaterials(pScene, Filename);
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex>
	
}