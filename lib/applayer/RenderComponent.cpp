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

#include "applayer/RenderComponent.h"

#include "applayer/Actor.h"
#include "applayer/MeshComponent.h"
#include <renderer/Mesh.h>
#include <renderer/RenderObject.h>

using namespace GCL;


GCL::RenderComponent::RenderComponent(Actor *parentActor, PtrLuaTableIterator &)
	: Component(parentActor)
{

}

void GCL::RenderComponent::PostInit()
{
	//must find the mesh component and hook it up with us
	Component *meshComponent = mParentActor->GetComponent("MeshComponent");
	MeshComponent *tempMeshComponent = static_cast<MeshComponent*>(meshComponent);

	Mesh &mesh = tempMeshComponent->GetMesh();
	switch ((size_t)mesh.GetVertexType())
	{
	case ePOSITION:
		mObj = new RenderObject(mesh.GetMaterial(), 
								(const VertexP*)mesh.GetVertexData(0), 
								mesh.GetVertexCount(0));
		break;
	case ePOSITION|eNORMAL:
		mObj = new RenderObject(mesh.GetMaterial(), 
								(const VertexPN*)mesh.GetVertexData(0), 
								mesh.GetVertexCount(0));
		break;
	case ePOSITION|eNORMAL|eTEXTURE_COORD:
		mObj = new RenderObject(mesh.GetMaterial(), 
								(const VertexPNT*)mesh.GetVertexData(0), 
								mesh.GetVertexCount(0));
		break;
	default:
		GCLAssert(false);
	}
}

void GCL::RenderComponent::ProcessEvent( size_t , void * )
{

}

void GCL::RenderComponent::Render()
{
	const RenderObject *tempRenderObject = mObj;
	const Material &tempMaterial = tempRenderObject->GetMaterial();
	tempMaterial.Bind();
	const Matrix44 &transform = mParentActor->GetTransform();
	GPUProgram *tempProgram = tempMaterial.GetShader();
	tempProgram->SetProjectionMatrix();
	tempProgram->SetModelViewMatrix(transform);
	tempRenderObject->GetVBO().Render();
}
