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

#include "applayer/MeshComponent.h"
#include "renderer/Mesh.h"

using namespace GCL;

 std::pair<const char *, Component *> MeshComponent::Create(Actor *parentActor,PtrLuaTableIterator &it)
{
	return  std::pair<const char *, Component *>("MeshComponent", new MeshComponent(parentActor, it));
}


GCL::MeshComponent::MeshComponent(Actor *parentActor, PtrLuaTableIterator &it)
	: Component(parentActor)
{
	while (!it->End())
	{
		if (it->GetKey() == "filename")
		{
			const std::string meshPath = MESH_PATH + it->GetString();
			mMesh = new Mesh(meshPath.c_str());
		}
		++(*it);
	}
}
