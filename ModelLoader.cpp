#include "ModelLoader.h"


ModelLoader::ModelLoader(void)
{
}


ModelLoader::~ModelLoader(void)
{

}

bool ModelLoader::LoadMesh(const std::string filename)
{
	bool Ret = false; 
	Assimp::Importer Importer; 

	const aiScene* pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if(pScene)
	{
		InitFromScene(pScene, filename);
		Ret = true;
	}
	else
	{
		printf("Error parsing '%s': '%s'\n", filename.c_str(), Importer.GetErrorString());
	}
	return Ret;
}

void ModelLoader::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	m_Entries.resize(pScene->mNumMeshes);

	//init meshes in scene
	for(unsigned int i = 0; i < m_Entries.size() ; i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}
}

void ModelLoader::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for(unsigned int i = 0; i < paiMesh->mNumVertices;i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
			glm::vec2(pTexCoord->x, pTexCoord->y),
			glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		m_data.push_back(v);
	}

	for(unsigned int i = 0; i < paiMesh->mNumFaces ; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i]; 
		assert(Face.mNumIndices == 3);
		m_indices.push_back(Face.mIndices[0]);
		m_indices.push_back(Face.mIndices[1]);
		m_indices.push_back(Face.mIndices[2]);
	}
}