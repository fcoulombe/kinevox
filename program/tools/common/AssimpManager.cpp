/*
* Copyright (C) 2011 by Francois Coulombe
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "common/AssimpManager.h"

using namespace GCL;
#if 0
#define LOG_ASSIMP(...) printf(__VA_ARGS__)
#else
#define LOG_ASSIMP(...)
#endif
void GCL::AssimpManager::Initialize()
{

}

void GCL::AssimpManager::Terminate()
{

}

static ToolMeshData sMeshData;
static ToolNodeData sNodeData;
void ParseNode(const aiScene* scene, const aiNode* node, ToolNodeData &nodeData)
{
	
	LOG_ASSIMP("   %s \n", node->mName.C_Str());
	nodeData.mName = node->mName.C_Str();
	nodeData.mTransform[0] = WorldPoint4(node->mTransformation[0][0], node->mTransformation[0][1], node->mTransformation[0][2], node->mTransformation[0][3]);
	nodeData.mTransform[1] = WorldPoint4(node->mTransformation[1][0], node->mTransformation[1][1], node->mTransformation[1][2], node->mTransformation[1][3]);
	nodeData.mTransform[2] = WorldPoint4(node->mTransformation[2][0], node->mTransformation[2][1], node->mTransformation[2][2], node->mTransformation[2][3]);
	nodeData.mTransform[3] = WorldPoint4(node->mTransformation[3][0], node->mTransformation[3][1], node->mTransformation[3][2], node->mTransformation[3][3]);
	nodeData.mNodeList.resize(node->mNumChildren);
	ToolMeshData meshData;
	for (size_t i=0; i<node->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ToolSubMeshData subMeshData;
		if(mesh->mName.length == 0)
			continue;
		LOG_ASSIMP("   %s \n", mesh->mName.C_Str());
		LOG_ASSIMP ("    %i vertices in mesh[%d]\n", mesh->mNumVertices, i);
		LOG_ASSIMP ("    %i uv comp in mesh[%d]\n", mesh->mNumUVComponents, i);
		LOG_ASSIMP ("    %i uv channels in mesh[%d]\n", mesh->GetNumUVChannels(), i);
		LOG_ASSIMP ("    %i faces in mesh[%d]\n", mesh->mNumFaces, i);
		LOG_ASSIMP ("    %i color channels in mesh[%d]\n", mesh->GetNumColorChannels(), i);
		LOG_ASSIMP ("    %i bones in mesh[%d]\n", mesh->mNumBones, i);
		for (size_t j=0; j<mesh->mNumFaces; ++j)
		{
			LOG_ASSIMP("Face: %d\n", j);
			const aiFace &tempFace = mesh->mFaces[j];
			for (size_t k =0; k<tempFace.mNumIndices; ++k)
			{
				unsigned int indice = tempFace.mIndices[k];
				const aiVector3D &position = mesh->mVertices[indice];
				LOG_ASSIMP("pos: %f %f %f\n", position.x, position.y, position.z);
				const aiVector3D &texCoord = mesh->mTextureCoords[0][indice];
				LOG_ASSIMP("uv: %f %f %f\n", texCoord.x, texCoord.y, texCoord.z);
				const aiVector3D &normal = mesh->mNormals[indice];
				LOG_ASSIMP("normal: %f %f %f\n", normal.x, normal.y, normal.z);
				aiColor4D color;
				if (mesh->GetNumColorChannels())
					color = mesh->mColors[0][indice];
				subMeshData.mIndiceList.push_back(indice);
				subMeshData.mVertexList.push_back(Point3<MeshReal>(position.x, position.z, position.y));
				subMeshData.mUvList.push_back(Point2<MeshReal>(texCoord.x, texCoord.y));
				subMeshData.mNormalList.push_back(Point3<MeshReal>(normal.x, normal.y, normal.z));
				subMeshData.mVertexColor.push_back(Point4<MeshReal>(color.r, color.g, color.b, color.a));
			}
		}
		meshData.mSubMeshList.push_back(subMeshData);
//		meshData.mMaterialData
//		meshData.mMaterialCount
//		mesh->mMaterialIndex
	}
	sMeshData = meshData;
	for (size_t i=0; i<node->mNumChildren; ++i)
	{
		const aiNode *tempNode = node->mChildren[i];
		ParseNode(scene, tempNode, nodeData.mNodeList[i]);
	}
}

bool GCL::AssimpManager::LoadScene( const char *filename )
{
	const aiScene* scene = aiImportFile (filename, 0/*aiProcess_Triangulate*/);
	
	GCLAssertMsg(scene, std::string("ERROR: reading mesh ") + filename);
	LOG_ASSIMP ("  %i animations\n", scene->mNumAnimations);
	LOG_ASSIMP ("  %i cameras\n", scene->mNumCameras);
	LOG_ASSIMP ("  %i lights\n", scene->mNumLights);
	LOG_ASSIMP ("  %i materials\n", scene->mNumMaterials);
	LOG_ASSIMP ("  %i meshes\n", scene->mNumMeshes);
	LOG_ASSIMP ("  %i textures\n", scene->mNumTextures);
	aiNode* node = scene->mRootNode;
	ParseNode(scene, node, sNodeData);
	for (size_t i=0; i<scene->mNumMaterials; ++i)
	{
		aiMaterial *tempMaterial = scene->mMaterials[i];
		aiString name;
		tempMaterial->Get(AI_MATKEY_NAME, name);
		int isTwoSided;
		tempMaterial->Get(AI_MATKEY_TWOSIDED, isTwoSided);
		int shadingModel;
		tempMaterial->Get(AI_MATKEY_SHADING_MODEL, shadingModel);
		int isWireFrameEnabled;
		tempMaterial->Get(AI_MATKEY_ENABLE_WIREFRAME, isWireFrameEnabled);
		int blendFunc;
		tempMaterial->Get(AI_MATKEY_BLEND_FUNC, blendFunc);
		float opacity;
		tempMaterial->Get(AI_MATKEY_OPACITY, opacity);
		float shininess;
		tempMaterial->Get(AI_MATKEY_SHININESS, shininess);
		float shininesStrength;
		tempMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, shininesStrength);
		float refracti;
		tempMaterial->Get(AI_MATKEY_REFRACTI, refracti);
		aiColor3D diffuse;
		tempMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aiColor3D ambient;
		tempMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		aiColor3D specular;
		tempMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		aiColor3D emissive;
		tempMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
		aiColor3D transparent;
		tempMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent);
		ToolMaterialData materialData;
		
		materialData.mAmbient = WorldPoint4(ambient.r, ambient.g, ambient.b, 1.0);
		materialData.matName = name.C_Str();
		materialData.mDiffuse = WorldPoint4(diffuse.r, diffuse.g, diffuse.b, 1.0);
		materialData.mEmissive = WorldPoint4(emissive.r, emissive.g, emissive.b, 1.0);
		materialData.mShininess = shininess;
		materialData.mSpecular = WorldPoint4(specular.r, specular.g, specular.b, 1.0);
		//tempMaterial->GetTextureCount()
		aiString texturePath;
		tempMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
		materialData.texName = texturePath.C_Str();
		sMeshData.mMaterialData = materialData;
		sMeshData.mMaterialCount = 1;
	}

	return false;
}

ToolMeshData & GCL::AssimpManager::GetMeshData()
{
	return sMeshData;
}
ToolNodeData & GCL::AssimpManager::GetNodeData()
{
	return sNodeData;
}
