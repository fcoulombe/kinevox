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
#ifdef USE_FBX
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
    FbxString lPath = (Path::FbxGetApplicationDirectory());
    const char *cpath = lPath.Buffer();
#if defined(KARCH_ENV_WIN)
    KString lExtension = "dll";
#elif defined(KARCH_ENV_MACOSX)
    KString lExtension = "dylib";
#elif defined(KARCH_ENV_LINUX)
    KString lExtension = "so";
#endif
    bool res = pSdkManager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());
    GCLAssert(res);
    // Create the entity that will hold the scene.
    pScene = KFbxScene::Create(pSdkManager,"");
    GCLAssert(pScene);
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
    GCLAssert(lImporter);
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
    EErrorCode err = FbxIOBase::GetLastErrorID();
    // Destroy the importer.
    //lImporter->Destroy();

    return lStatus;
}

WorldPoint4 GetColor( const FbxGeometry * tempMesh, int lControlPointIndex, int vertexId ) 
{
    FbxColor color;

    int vtxColorCount = tempMesh->GetElementVertexColorCount();
    GCLAssert(vtxColorCount <= 1);
    for (int l = 0; l < vtxColorCount; l++)
    {
        const FbxGeometryElementVertexColor* leVtxc = tempMesh->GetElementVertexColor( l);

        switch (leVtxc->GetMappingMode())
        {
        case FbxGeometryElement::eByControlPoint:
            switch (leVtxc->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
                color = leVtxc->GetDirectArray().GetAt(lControlPointIndex);
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
                    color = leVtxc->GetDirectArray().GetAt(id);
                }
                break;
            default:
                break; // other reference modes not shown here!
            }
            break;
        case FbxGeometryElement::eByPolygonVertex:
            {
                switch (leVtxc->GetReferenceMode())
                {
                case FbxGeometryElement::eDirect:
                    color = leVtxc->GetDirectArray().GetAt(vertexId);
                    break;
                case FbxGeometryElement::eIndexToDirect:
                    {
                        int id = leVtxc->GetIndexArray().GetAt(vertexId);
                        color = leVtxc->GetDirectArray().GetAt(id);
                    }
                    break;
                default:
                    break; // other reference modes not shown here!
                }
            }
            break;

        case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
        case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
        case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
            break;
        }
    }
    WorldPoint4 myColor;
    myColor.x = color.mRed;
    myColor.y = color.mGreen;
    myColor.z = color.mBlue;
    myColor.w = color.mAlpha;
    return myColor;
}

WorldPoint2 GetUV( FbxGeometry * tempMesh, int lControlPointIndex, int i, int j ) 
{
    FbxVector2 uv;
/*
    int uvCount = tempMesh->GetElementUVCount();
    GCLAssert(uvCount <= 1);
    for (int l = 0; l < uvCount; ++l)
    {
        const FbxGeometryElementUV* leUV = tempMesh->GetElementUV( l);

        switch (leUV->GetMappingMode())
        {
        case FbxGeometryElement::eByControlPoint:
            switch (leUV->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
                uv = leUV->GetDirectArray().GetAt(lControlPointIndex);
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
                    uv = leUV->GetDirectArray().GetAt(id);
                }
                break;
            default:
                break; // other reference modes not shown here!
            }
            break;
        case FbxGeometryElement::eByPolygonVertex:
            {
                int lTextureUVIndex = tempMesh->GetTextureUVIndex(i, j);
                switch (leUV->GetReferenceMode())
                {
                case FbxGeometryElement::eDirect:
                case FbxGeometryElement::eIndexToDirect:
                    {
                        uv=leUV->GetDirectArray().GetAt(lTextureUVIndex);
                    }
                    break;
                default:
                    break; // other reference modes not shown here!
                }
            }
            break;
        case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
        case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
        case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
            break;
        }
    }*/
    WorldPoint2 myUv;
    myUv.x = uv[0];
    myUv.y = uv[1];
    return myUv;
}

WorldPoint3 GetNormal( const FbxGeometry * tempMesh, int lControlPointIndex, int vertexId ) 
{
    FbxVector4 normal;
    size_t layerCount = tempMesh->GetLayerCount();
   const  FbxLayer *normalLayer = tempMesh->GetLayer(0, FbxLayerElement::eNormal);
   const FbxLayerElementNormal* normElem = normalLayer->GetNormals();
   switch(normElem->GetMappingMode())
   {
   case FbxLayerElement::eByControlPoint:
       switch (normElem->GetReferenceMode())
       {
       case FbxLayerElement::eDirect:
           {
           normal = normElem->GetDirectArray().GetAt(lControlPointIndex);
           }
           break;
       case FbxLayerElement::eIndexToDirect:
           {
           int id = normElem->GetIndexArray().GetAt(lControlPointIndex);
           normal = normElem->GetDirectArray().GetAt(id);
           }
           break;
       default:
           break;
       }
       break;
   case FbxLayerElement::eByPolygonVertex:
       switch (normElem->GetReferenceMode())
       {
       case FbxLayerElement::eDirect:
           {
           normal = normElem->GetDirectArray().GetAt(vertexId);
           }
           break;
       case FbxLayerElement::eIndexToDirect:
           {
           int id = normElem->GetIndexArray().GetAt(vertexId);
           normal = normElem->GetDirectArray().GetAt(id);
           }
           break;
       default:
           break;
       }
       break;
   default:
       GCLAssert(false);
       break;
   }
    int normalCount = tempMesh->GetElementNormalCount();
    GCLAssert(normalCount <= 1);
    for(int l = 0; l < normalCount; ++l)
    {
        const FbxGeometryElementNormal* leNormal = tempMesh->GetElementNormal( l);
        switch(leNormal->GetMappingMode())
        {
        case FbxGeometryElement::eByControlPoint:
            {
                switch (leNormal->GetReferenceMode())
                {
                case FbxGeometryElement::eDirect:
                    {
                        normal = leNormal->GetDirectArray().GetAt(lControlPointIndex);
                        break;
                    }
                case FbxGeometryElement::eIndexToDirect:
                    {
                        int id = leNormal->GetIndexArray().GetAt(lControlPointIndex);
                        normal = leNormal->GetDirectArray().GetAt(id);
                        break;
                    }
                default:
                    GCLAssert(false); // other reference modes not shown here!
                }
                break;
            }
        case FbxGeometryElement::eByPolygonVertex:
            {
                switch (leNormal->GetReferenceMode())
                {
                case FbxGeometryElement::eDirect:
                    {
                        normal = leNormal->GetDirectArray().GetAt(vertexId);
                        break;
                    }
                case FbxGeometryElement::eIndexToDirect:
                    {
                        int id = leNormal->GetIndexArray().GetAt(vertexId);
                        normal = leNormal->GetDirectArray().GetAt(id);
                        break;
                    }
                default:
                    GCLAssert(false);
                    break; // other reference modes not shown here!
                }
                break;
            }   
        default:
            GCLAssert(false);
        }
    }
    WorldPoint3 myNormal;
    myNormal.x = normal[0];
    myNormal.y = normal[1];
    myNormal.z = normal[2];
    return myNormal;
}



ToolMeshData FBXManager::GetMeshData3()
{
    ToolMeshData meshData;

    KFbxNode* lRootNode = pScene->GetRootNode();

    size_t childCount = lRootNode->GetChildCount();
    for (size_t i=0; i<childCount; ++i)
    {
        KFbxNode* lchild = lRootNode->GetChild(i);
        const char *nodeName = lchild->GetTypeName();
        std::cout << lchild->GetTypeName() << std::endl;;
        if (strcmp(nodeName, "Mesh")==0)
        {
          /*  meshData.mMaterialCount = lchild->GetMaterialCount();
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
            }*/

            //FbxGeometry *geom = lchild->GetGeometry();
            
            KFbxMesh *tempMesh = lchild->GetMesh();
            FbxVector4* lControlPoints = tempMesh->GetControlPoints(); 
            int vertexId = 0;
            size_t polyGrp = tempMesh->GetElementPolygonGroupCount();
            std::cout << "polygrp: " << polyGrp<<std::endl;
            std::cout << "ctrl ptr: " << tempMesh->GetControlPointsCount()<<std::endl;
            std::cout << "dst obj: " << tempMesh->GetDstObjectCount()<<std::endl;
            std::cout << "dst prop: " << tempMesh->GetDstPropertyCount()<<std::endl;
            std::cout << "src obj: " << tempMesh->GetSrcObjectCount()<<std::endl;
            std::cout << "src prop: " << tempMesh->GetSrcPropertyCount()<<std::endl;
            std::cout << "elem normal: " << tempMesh->GetElementNormalCount()<<std::endl;
            //std::cout << "\tdirect: " << tempMesh->GetElementNormal()->GetDirectArray().GetCount()<<std::endl;
            std::cout << "\tindex: " << tempMesh->GetElementNormal()->GetIndexArray().GetCount()<<std::endl;
            for (size_t i=0; i<tempMesh->GetElementNormal()->GetIndexArray().GetCount(); ++i)
            {
                //FbxVector4 n 
                FbxGeometryElementNormal *en = tempMesh->GetElementNormal();
                FbxLayerElementArrayTemplate<int> &ena = en->GetIndexArray();
                FbxArray<int> arr;
                ena.CopyTo(arr);
                int n = ena.GetAt(i);
                std::cout << "\t\tnorm: " << n<< std::endl;
                //std::cout << "\t\tnorm: " << n[0] << " " << n[1] << " " << n[3] << " " << n[4] << std::endl;
            }

            std::cout << "elem uv: " << tempMesh->GetElementUVCount()<<std::endl;
            std::cout << "elem vtc: " << tempMesh->GetElementVertexColorCount()<<std::endl;
            std::cout << "layer: " << tempMesh->GetLayerCount()<<std::endl;
            std::cout << "polygon: " << tempMesh->GetPolygonCount()<<std::endl;
            std::cout << "poly vertx: " << tempMesh->GetPolygonVertexCount()<<std::endl;
            std::cout << "shape: " << tempMesh->GetShapeCount()<<std::endl;
            std::cout << "texture uv: " << tempMesh->GetTextureUVCount()<<std::endl;
            /*int lPolygonCount = tempMesh->GetPolygonCount();
            int *indiceList = tempMesh->GetPolygonVertices();
            meshData.mIndicesCount = tempMesh->GetPolygonVertexCount();
            for (int i=0; i<meshData.mIndicesCount; ++i)
            {
                int lControlPointIndex = indiceList[i];
                WorldPoint4 position;
                position.x = lControlPoints[lControlPointIndex][0];
                position.y = lControlPoints[lControlPointIndex][2];
                position.z = lControlPoints[lControlPointIndex][1];
                position.w = lControlPoints[lControlPointIndex][3];
                meshData.mVertexList.push_back(position);
                WorldPoint4 color = GetColor(tempMesh, lControlPointIndex, vertexId);
                WorldPoint3 normal = GetNormal(tempMesh, lControlPointIndex, vertexId);
                meshData.mNormalList.push_back(normal);
                //                meshData.mUvList.push_back(uv);
                meshData.mVertexColor.push_back(color);
                meshData.mIndiceList.push_back(lControlPointIndex);

                vertexId++;
            }
            for (int i = 0; i < lPolygonCount; i++)
            {
                int polygonGrpCount = tempMesh->GetElementPolygonGroupCount();
                for (int l = 0; l < polygonGrpCount; l++)
                {
                    const FbxGeometryElementPolygonGroup* lePolgrp = tempMesh->GetElementPolygonGroup(l);
                    GCLAssertMsg(lePolgrp->GetMappingMode()==FbxGeometryElement::eByPolygon &&
                        lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex, 
                        "unsupported group assignment");

                    int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
                }

                int lPolygonSize = tempMesh->GetPolygonSize(i);
                for (int j = 0; j < lPolygonSize; j++)
                {
                    int lControlPointIndex = tempMesh->GetPolygonVertex(i, j);

                    WorldPoint4 position;
                    position.x = lControlPoints[lControlPointIndex][0];
                    position.y = lControlPoints[lControlPointIndex][2];
                    position.z = lControlPoints[lControlPointIndex][1];
                    position.w = lControlPoints[lControlPointIndex][3];
                    meshData.mVertexList.push_back(position);
                    WorldPoint4 color = GetColor(tempMesh, lControlPointIndex, vertexId);
                    WorldPoint2 uv = GetUV(tempMesh, lControlPointIndex, i, j);
                    WorldPoint3 normal = GetNormal(tempMesh, lControlPointIndex, vertexId);
                    meshData.mNormalList.push_back(normal);
                    meshData.mUvList.push_back(uv);
                    meshData.mVertexColor.push_back(color);
                    meshData.mIndiceList.push_back(lControlPointIndex);
                    vertexId++;
                } // for polygonSize
            } // for polygonCount*/
        }
    }
    return meshData;
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

#endif