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







void SaveMesh(const MeshData &mesh, const char *filename)
{

	(void)mesh;
	(void)filename;
}

int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{
		FBXManager::Initialize();

	    FBXManager::LoadScene("data/ExampleMesh.fbx");

	    KFbxScene* pScene = FBXManager::GetScene();
	    (void)pScene;

	    MeshData data = FBXManager::GetMeshData();
	    std::cout << data << std::endl;

		SaveMesh(data, "data/ExampleMesh.mesh");

		FBXManager::Terminate();

	}
	catch(GCLException &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

