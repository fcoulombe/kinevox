/*
 * Copyright (C) 2014 by Francois Coulombe
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

#include "applayer/GCLWorld.h"
#include "applayer/Node.h"
#include <gcl/Assert.h>
#include <gcl/File.h>

using namespace GCL;


GCL::GCLWorld::GCLWorld( const char * name )
{
	std::string worldFile(WORLD_PATH);
	worldFile += name;
	worldFile += ".world";
	GCLAssert(GCLFile::Exists(worldFile));
	GCLFile fp(worldFile);
	const size_t fileSize = fp.GetFileSize();
	mWorldFileData = new uint8_t[fileSize];
	fp.Read(mWorldFileData, fileSize);
	NodeData *data = (NodeData *)mWorldFileData;
	mRootNode = new Node(data);

}

GCL::GCLWorld::GCLWorld()
{
	mWorldFileData = nullptr;
	mRootNode = new Node("World");
}

GCL::GCLWorld::~GCLWorld()
{
	delete mRootNode;
	delete [] mWorldFileData;
}
