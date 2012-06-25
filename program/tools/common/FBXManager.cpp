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

#include "common/FBXManager.h"
#include <gcl/Assert.h>
#include <gcl/Path.h>

#include <iostream>
#include <sstream>

using namespace GCL;

KFbxSdkManager* FBXManager::pSdkManager = NULL;
KFbxScene* FBXManager::pScene = NULL;

void FBXManager::Initialize()
{
	//std::cout << "FBXManager::Initialize" << std::endl;
	pSdkManager = KFbxSdkManager::Create();

	GCLAssert(pSdkManager && "Unable to create the FBX SDK manager\n");

	// create an IOSettings object
	KFbxIOSettings * ios = KFbxIOSettings::Create(pSdkManager, IOSROOT );
	pSdkManager->SetIOSettings(ios);

	// Load plugins from the executable directory
	//KString lPath = KFbxGetApplicationDirectory();
	KString lPath(Path::GetApplicationPath().c_str());
#if defined(KARCH_ENV_WIN)
	KString lExtension = "dll";
#elif defined(KARCH_ENV_MACOSX)
	KString lExtension = "dylib";
#elif defined(KARCH_ENV_LINUX)
	KString lExtension = "so";
#endif
	pSdkManager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());

	// Create the entity that will hold the scene.
	pScene = KFbxScene::Create(pSdkManager,"");

}


void FBXManager::Terminate()
{
	//std::cout << "FBXManager::Terminate()" << std::endl;
	GCLAssert(pSdkManager);
	pSdkManager->Destroy();
	pSdkManager = NULL;
}


bool FBXManager::LoadScene(const char* pFilename)
{
	//std::cout << "FBXManager::LoadScene()" << std::endl;

	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor,  lSDKMinor,  lSDKRevision;
	//int lFileFormat = -1;
	//int i, lAnimStackCount;
	bool lStatus;

	// Get the file version number generate by the FBX SDK.
	KFbxSdkManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	KFbxImporter* lImporter = KFbxImporter::Create(pSdkManager,"");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pSdkManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	std::stringstream s;
	s<< "Call to KFbxImporter::Initialize() failed." << std::endl;
	s<<"Error returned: " <<  lImporter->GetLastErrorString() << std::endl<<std::endl;
	if (lImporter->GetLastErrorID() == FbxIOBase::eFileVersionNotSupportedYet ||
			lImporter->GetLastErrorID() == FbxIOBase::eFileVersionNotSupportedAnymore)
	{
		s<<"FBX version number for this FBX SDK is " << lSDKMajor <<"."<< lSDKMinor <<"."<< lSDKRevision<<std::endl;
		s<<"FBX version number for file " << pFilename << " is " << lFileMajor <<"."<< lFileMinor<<"."<<lFileRevision;
	}
	s<<"while loading: " << pFilename << std::endl;

	GCLAssertMsg(lImportStatus, s.str().c_str() );

	//printf("FBX version number for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		//printf("FBX version number for file %s is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		/*printf("Animation Stack Information\n");

		lAnimStackCount = lImporter->GetAnimStackCount();

		printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		printf("\n");

		for(i = 0; i < lAnimStackCount; i++)
		{
			KFbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			printf("    Animation Stack %d\n", i);
			printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported
			// under a different name.
			printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported.
			printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			printf("\n");
		}
		 */
		// Set the import states. By default, the import states are always set to
		// true. The code below shows how to change these states.
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);

	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}


ToolMeshData FBXManager::GetMeshData()
{
	ToolMeshData meshData;

	KFbxNode* lRootNode = pScene->GetRootNode();

	//std::cout << lRootNode->GetTypeName() << std::endl;
	size_t childCount = lRootNode->GetChildCount();
	for (size_t i=0; i<childCount; ++i)
	{
		KFbxNode* lchild = lRootNode->GetChild(i);
		//std::cout << lchild->GetTypeName() << std::endl;;
		if (strcmp(lchild->GetTypeName(), "Mesh")==0)
		{

			const KFbxMesh *tempMesh = lchild->GetMesh();

			const KFbxLayer* layer = tempMesh->GetLayer(0);

			const FbxLayerElementArrayTemplate<KFbxVector4> &normals = layer->GetNormals()->GetDirectArray();
			//const KFbxLayerElementVertexColor* vertexColor = layer->GetVertexColors()->GetDirectArray();

			meshData.mMaterialCount = lchild->GetMaterialCount();
			for (size_t i=0; i<meshData.mMaterialCount; ++i)
			{
				const KFbxSurfaceMaterial* tempMat = lchild->GetMaterial(i);

				std::string matFullName = tempMat->GetName() ;
				size_t pos = matFullName.find("__");
				std::string matName = matFullName.substr(0, pos);
				std::string texName = matFullName.substr(pos+2, matFullName.length());
				texName[texName.length()-4]='.';
				meshData.mMaterialData.matName = (matName);
				meshData.mMaterialData.texName = (texName);
			}

			meshData.mIndicesCount = tempMesh->GetPolygonVertexCount();

			meshData.mVertexCount = tempMesh->GetControlPointsCount();
			//std::cout << "ElemPolygonGroupCount: " << tempMesh->GetElementPolygonGroupCount()<<std::endl;
			//std::cout << "ElemUVCount: " << tempMesh->GetElementUVCount()<<std::endl;
			//std::cout << "ElemVertexColorCount: " << tempMesh->GetElementVertexColorCount()<<std::endl;

			//tempMesh->GetPolygonVertexIndex();
			meshData.mNormalCount = normals.GetCount();
			for (int i=0; i<normals.GetCount(); ++i)
			{
				WorldPoint3 tempNormal;
				tempNormal.x = normals[i][0];
				tempNormal.y = normals[i][2];
				tempNormal.z = normals[i][1];
				meshData.mNormalList.push_back(tempNormal);
			}

			const FbxArray<KFbxLayerElementUV const*> &uvst = layer->GetUVSets();
			KFbxLayerElementUV const* uvs = uvst[0];
			meshData.mUvCount = uvs->GetDirectArray().GetCount();
			for (int i=0; i<uvs->GetDirectArray().GetCount(); ++i)
			{
				WorldPoint2 tempUv;
				const KFbxVector2 &tempFbxUv =uvs->GetDirectArray()[i];
				tempUv.x = tempFbxUv[0];
				tempUv.y = tempFbxUv[1];
				meshData.mUvList.push_back(tempUv);
			}
			meshData.mUvCount  = meshData.mUvList.size();
			KFbxVector4* vertices = tempMesh->GetControlPoints();

			for (int i=0; i<tempMesh->GetControlPointsCount(); ++i)
			{
				WorldPoint4 position;
				position.x = vertices[i][0];
				position.y = vertices[i][2];
				position.z = vertices[i][1];
				position.w = vertices[i][3];
				meshData.mVertexList.push_back(position);
			}

			int *polygonVertices = tempMesh->GetPolygonVertices();
			for (int i=0; i<tempMesh->GetPolygonVertexCount(); ++i)
			{
				meshData.mIndiceList.push_back(polygonVertices[i]);
			}

			/*        	tempMesh->GetPolygonVertexUVs();
        	tempMesh->Get*/
		}
	}

	return meshData;
}
