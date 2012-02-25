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

#include <iostream>

using namespace GCL;

KFbxSdkManager* FBXManager::pSdkManager = NULL;
KFbxScene* FBXManager::pScene = NULL;

void FBXManager::Initialize()
{
	std::cout << "FBXManager::Initialize" << std::endl;
	pSdkManager = KFbxSdkManager::Create();

	GCLAssert(pSdkManager && "Unable to create the FBX SDK manager\n");

	// create an IOSettings object
	KFbxIOSettings * ios = KFbxIOSettings::Create(pSdkManager, IOSROOT );
	pSdkManager->SetIOSettings(ios);

	// Load plugins from the executable directory
	KString lPath = KFbxGetApplicationDirectory();
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
	std::cout << "FBXManager::Terminate()" << std::endl;
	GCLAssert(pSdkManager);
	pSdkManager->Destroy();
	pSdkManager = NULL;
}


bool FBXManager::LoadScene(const char* pFilename)
{
	std::cout << "FBXManager::LoadScene()" << std::endl;

	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor,  lSDKMinor,  lSDKRevision;
	//int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;

	// Get the file version number generate by the FBX SDK.
	KFbxSdkManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	KFbxImporter* lImporter = KFbxImporter::Create(pSdkManager,"");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pSdkManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if( !lImportStatus )
	{
		printf("Call to KFbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetLastErrorString());

		if (lImporter->GetLastErrorID() == KFbxIO::eFILE_VERSION_NOT_SUPPORTED_YET ||
				lImporter->GetLastErrorID() == KFbxIO::eFILE_VERSION_NOT_SUPPORTED_ANYMORE)
		{
			printf("FBX version number for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			printf("FBX version number for file %s is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	printf("FBX version number for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		printf("FBX version number for file %s is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		printf("Animation Stack Information\n");

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


MeshData FBXManager::GetMeshData()
{
	MeshData meshData;

	KFbxNode* lRootNode = pScene->GetRootNode();

	std::cout << lRootNode->GetTypeName() << std::endl;
	size_t childCount = lRootNode->GetChildCount();
	for (size_t i=0; i<childCount; ++i)
	{
		KFbxNode* lchild = lRootNode->GetChild(i);
		std::cout << lchild->GetTypeName() << std::endl;;
		if (strcmp(lchild->GetTypeName(), "Mesh")==0)
		{

			KFbxMesh *tempMesh = lchild->GetMesh();
			meshData.mMaterialCount = tempMesh->GetElementMaterialCount();
			meshData.mNormalCount = tempMesh->GetElementNormalCount();
			meshData.mVertexCount = tempMesh->GetPolygonVertexCount();
			//std::cout << "ElemPolygonGroupCount: " << tempMesh->GetElementPolygonGroupCount()<<std::endl;
			//std::cout << "ElemUVCount: " << tempMesh->GetElementUVCount()<<std::endl;
			//std::cout << "ElemVertexColorCount: " << tempMesh->GetElementVertexColorCount()<<std::endl;

			//tempMesh->GetPolygonVertexIndex();
			KArrayTemplate<KFbxVector4> pNormals;
			tempMesh->GetPolygonVertexNormals(pNormals);
			GCLAssert(meshData.mNormalCount);

			for (int i=0; i<pNormals.GetCount(); ++i)
			{
				WorldPoint3 tempNormal;
				tempNormal.x = pNormals[i][0];
				tempNormal.y = pNormals[i][1];
				tempNormal.z = pNormals[i][2];
				meshData.mNormalList.push_back(tempNormal);
			}
			KFbxVector4* vertices = tempMesh->GetControlPoints();

			for (int i=0; i<tempMesh->GetControlPointsCount(); ++i)
			{
				WorldPoint4 position;
				position.x = vertices[i][0];
				position.y = vertices[i][1];
				position.z = vertices[i][2];
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