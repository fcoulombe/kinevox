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

#include <unistd.h>
#include <cstdlib>
#include <sstream>
#include <vector>

#include <gcl/Exception.h>
#include <gcl/Point3.h>
#include <gcl/Point4.h>

#include <common/FBXManager.h>

using namespace GCL;



struct MeshData
{
	size_t mVertexCount;
	std::vector<WorldPoint4> mVertexList;

	size_t mNormalCount;
	std::vector<WorldPoint3> mNormalList;

	size_t mVertexColorCount;
	std::vector<WorldPoint3> mVertexColor;

	size_t mMaterialCount;
};


MeshData *LoadFBXFile(const char *filename)
{
	FBXManager::Initialize();

    FBXManager::LoadScene(filename);

    KFbxScene* pScene = FBXManager::GetScene();

    KFbxNode* lRootNode = pScene->GetRootNode();

    std::cout << lRootNode->GetTypeName() << std::endl;
    size_t childCount = lRootNode->GetChildCount();
    for (size_t i=0; i<childCount; ++i)
    {
        KFbxNode* lchild = lRootNode->GetChild(i);
        std::cout << lchild->GetTypeName() << std::endl;;
        if (strcmp(lchild->GetTypeName(), "Mesh")==0)
        {
        	MeshData meshData;

        	std::cout << "we have a mesh" << std::endl;
        	KFbxMesh *tempMesh = lchild->GetMesh();
        	std::cout << "MaterialCount: " << tempMesh->GetElementMaterialCount() <<std::endl;
        	meshData.mMaterialCount = tempMesh->GetElementMaterialCount();

        	std::cout << "ElemNormCount: " << tempMesh->GetElementNormalCount()<<std::endl;
        	std::cout << "ElemPolygonGroupCount: " << tempMesh->GetElementPolygonGroupCount()<<std::endl;
        	std::cout << "ElemUVCount: " << tempMesh->GetElementUVCount()<<std::endl;
        	std::cout << "ElemVertexColorCount: " << tempMesh->GetElementVertexColorCount()<<std::endl;
        	std::cout << "PolyVertexCount: " << tempMesh->GetPolygonVertexCount()<<std::endl;
          	//tempMesh->GetPolygonVertexIndex();
        	KArrayTemplate<KFbxVector4> pNormals;
        	tempMesh->GetPolygonVertexNormals(pNormals);
        	std::cout << "Normals"<< std::endl;
        	for (int i=0; i<pNormals.GetCount(); ++i)
        	{
        		std::cout << pNormals[i][0] << " " << pNormals[i][1] << " " << pNormals[i][2] << std::endl;
        	}
        	KFbxVector4* positions = tempMesh->GetControlPoints();
        	std::cout << "Position" << std::endl;
        	for (int i=0; i<tempMesh->GetControlPointsCount(); ++i)
        	{
        		std::cout << positions[i][0] << " " << positions[i][1] << " " << positions[i][2] << std::endl;
        	}

        	std::cout << "Polygons" << std::endl;

        	int *polygonVertices = tempMesh->GetPolygonVertices();
        	for (int i=0; i<tempMesh->GetPolygonVertexCount(); ++i)
        	{
        		std::cout << polygonVertices[i] << std::endl;
        	}

/*        	tempMesh->GetPolygonVertexUVs();
        	tempMesh->Get*/
        }

        for (int i=0; i<lchild->GetChildCount(); ++i)
        {
            KFbxNode* lchild2 = lchild->GetChild(i);
            std::cout << lchild2->GetTypeName() << std::endl;;
        }
    }

FBXManager::Terminate();

	return NULL;
}

void SaveMesh(MeshData *mesh, const char *filename)
{

	(void)mesh;
	(void)filename;
}

int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{
		MeshData *mesh = LoadFBXFile("data/ExampleMesh.fbx");
		SaveMesh(mesh, "data/ExampleMesh.mesh");
		delete mesh;
	}
	catch(GCLException &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

