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

#include <cstdlib>
#include <sstream>
#include <vector>

#include <gcl/Exception.h>
#include <gcl/Point3.h>
#include <gcl/Point4.h>

#include <common/FBXManager.h>

using namespace GCL;


int main(int /*argc*/, char ** argv)
{
	//std::cout << "start program" << std::endl;
	//std::cout << "Mesh Convert from: " << argv[1] << " to: " << argv[2] << std::endl;
	try
	{
		FBXManager::Initialize();

	    FBXManager::LoadScene(argv[1]);

	    KFbxScene* pScene = FBXManager::GetScene();
	    (void)pScene;

	    ToolMeshData data = FBXManager::GetMeshData();
	  //  std::cout << data << std::endl;

	    BufferWriter buffer(1024*1500);
	    buffer<<data;

	    const std::string outputMeshFileName(argv[2]);
	    buffer.WriteToFile(outputMeshFileName);
#if OS_WIN32
        size_t pos = outputMeshFileName.find("data\\Mesh");
#else
	    size_t pos = outputMeshFileName.find("data/Mesh");
#endif
        GCLAssert(pos != std::string::npos);
	    const std::string outputMatFileName = outputMeshFileName.substr(0, pos)+"data/Material/" + data.mMaterialData.matName+".mat";
		data.mMaterialData.WriteToFile(outputMatFileName);
		FBXManager::Terminate();

	}
	catch(GCLException &e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}

