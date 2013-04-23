/*
 * Copyright (C) 2013 by Francois Coulombe
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
#pragma once
#include <sstream>

#include <common/SceneLoader.h>
#include <gcl/UnitTest.h>

using namespace GCL;
namespace MeshSavingTest
{


void Test()
{
	TEST_START

	SceneLoader::Initialize();

	SceneLoader::LoadScene("datamisc/ExampleMesh.dae");

	ToolMeshData &data = SceneLoader::GetMeshData();
	//std::cout << data << std::endl;

	BufferWriter buffer(1024*500);
	buffer << data;

	std::stringstream s;
	{
		s.str("");
		ToolMeshData testData;
        testData.mMaterialCount = 1;
        
        ToolSubMeshData subMeshTestData;
		subMeshTestData.mVertexList.resize(8);
		subMeshTestData.mIndiceList.resize(36);
		subMeshTestData.mNormalList.resize(8);
		subMeshTestData.mVertexColor.resize(0);
		subMeshTestData.mUvList.resize(36);
        testData.mSubMeshList.push_back(subMeshTestData);

		ToolMeshData *testData2 = reinterpret_cast<ToolMeshData*>(buffer.GetBuffer());
		(void)testData2;
		#if 0
        s<< testData.mSubMeshList.size() << " == " << testData2->mSubMeshList.size() << std::endl;
        AssertMsg_Test(testData.mSubMeshList.size()  == testData2->mSubMeshList.size(), s.str().c_str());
        s.str("");
//GCLAssert(false); //unimp
 
ToolSubMeshData

		s<< testData.mVertexCount << " == " << testData2->mVertexList.size() << std::endl;
		AssertMsg_Test(testData.mVertexCount  == testData2->mVertexList.size(), s.str().c_str());
		s.str("");
		s<< testData.mIndicesCount << " == " << testData2->mIndicesCount << std::endl;
		AssertMsg_Test(testData.mIndicesCount  == testData2->mIndicesCount, s.str().c_str());
		s.str("");
		s<< testData.mNormalCount << " == " << testData2->mNormalCount << std::endl;
		AssertMsg_Test(testData.mNormalCount  == testData2->mNormalCount, s.str().c_str());
		s.str("");
		s<< testData.mVertexColorCount << " == " << testData2->mVertexColorCount << std::endl;
		//AssertMsg_Test(testData.mVertexColorCount  == testData2->mVertexColorCount, s.str().c_str());
		s.str("");
		s<< testData.mMaterialCount << " == " << testData2->mMaterialCount << std::endl;
		AssertMsg_Test(testData.mMaterialCount  == testData2->mMaterialCount, s.str().c_str());
		s.str("");
		s<< testData.mUvCount << " == " << testData2->mUvCount << std::endl;
		AssertMsg_Test(testData.mUvCount  == testData2->mUvCount, s.str().c_str());
#endif

		buffer.WriteToFile("ExampleMesh.mesh");
	}

	const std::string matFilename = data.mMaterialData.matName+".mat";
	data.mMaterialData.WriteToFile(matFilename);


	SceneLoader::Terminate();
}
}
