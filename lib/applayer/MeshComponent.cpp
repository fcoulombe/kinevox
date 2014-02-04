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
#include "applayer/Actor.h"
#include <renderer/Mesh.h>

using namespace GCL;



GCL::MeshComponent::MeshComponent(Actor *parentActor, PtrLuaTableIterator &it)
	: Component(parentActor)
{
	mMesh = nullptr;
	if (!it)
		return;
	while (!it->End())
	{
		if (it->GetKey() == "filename")
		{
			SetMesh(it->GetString());
		}
		++(*it);
	}
}

void MeshComponent::SetMesh(const std::string &filename)
{
	if (HasMesh())
		delete mMesh;
	const std::string meshPath = MESH_PATH + filename;
	mMesh = new Mesh(meshPath.c_str());
	if (mParentActor->HasComponent("RenderComponent"))
	{
		mParentActor->GetComponent("RenderComponent")->PostInit();
	}
}
GCL::MeshComponent::~MeshComponent()
{
	delete mMesh;
}
