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
#pragma once

#include <gcl/UnitTest.h>
#include <gcl/File.h>
#include <renderer/MeshResource.h>

using namespace GCL;

namespace MeshLoadingTest
{
void Test();
void Test()
{
	TEST_START

	GCLFile fp(MESH_PATH"ExampleMesh.mesh");
	const MeshResource::MeshData *testData2 = MeshResource::LoadMesh(fp);
    const MeshResource::SubMeshData *testSubMeshData2 = testData2->GetSubMeshData(0);
	Assert_Test(testData2);

	MeshResource::MeshData testData;
	testData.mSubMeshCount = 1;
    testData.mMaterialCount = 1;
    testData.mMaterialNameLen = 40;

    MeshResource::SubMeshData testSubMeshData;
    testSubMeshData.mSubMeshSize =  789492;
    testSubMeshData.mVertexCount = 21930;
	testSubMeshData.mIndicesCount = 21930;
	

	std::stringstream s;
    s.str("");
    s<< testData.mSubMeshCount<< " == " << testData2->mSubMeshCount << std::endl;
    AssertMsg_Test(testData.mSubMeshCount == testData2->mSubMeshCount, s.str().c_str());

    s.str("");
    s<< testData.mMaterialCount << " == " << testData2->mMaterialCount << std::endl;
    AssertMsg_Test(testData.mMaterialCount  == testData2->mMaterialCount, s.str().c_str());

    s.str("");
    s<< testData.mMaterialNameLen << " == " << testData2->mMaterialNameLen << " for mat: " << testData2->GetMaterialName() << std::endl;
    AssertMsg_Test(testData.mMaterialNameLen  == testData2->mMaterialNameLen, s.str().c_str());

    s.str("");
    s<< testSubMeshData.mSubMeshSize << " == " << testSubMeshData2->mSubMeshSize << std::endl;
    AssertMsg_Test(testSubMeshData.mSubMeshSize== testSubMeshData2->mSubMeshSize, s.str().c_str());
	s.str("");
    s<< testSubMeshData.mVertexCount << " == " << testSubMeshData2->mVertexCount << std::endl;
	AssertMsg_Test(testSubMeshData.mVertexCount  == testSubMeshData2->mVertexCount, s.str().c_str());
	s.str("");
	s<< testSubMeshData.mIndicesCount << " == " << testSubMeshData2->mIndicesCount << std::endl;
	AssertMsg_Test(testSubMeshData.mIndicesCount  == testSubMeshData2->mIndicesCount, s.str().c_str());
/*	s.str("");
	s<< testData.mNormalCount << " == " << testData2->mNormalCount << std::endl;
	AssertMsg_Test(testData.mNormalCount  == testData2->mNormalCount, s.str().c_str());
	s.str("");
	s<< testData.mVertexColorCount << " == " << testData2->mVertexColorCount << std::endl;
	//AssertMsg_Test(testData.mVertexColorCount  == testData2->mVertexColorCount, s.str().c_str());

    s.str("");
    s<< testData.mUvCount << " == " << testData2->mUvCount << std::endl;
    AssertMsg_Test(testData.mUvCount  == testData2->mUvCount, s.str().c_str());
    */



	MeshResource::Unload(testData2);
}
}
