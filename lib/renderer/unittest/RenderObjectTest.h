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
#include <renderer/Material.h>
#include <renderer/RenderObject.h>
#include <renderer/Shader.h>
#include <renderer/TextureResourceManager.h>
#include <renderer/Vertex.h>

using namespace GCL;
namespace RenderObjectTest
{
class MyRenderObject : public RenderObject
{
public:
	MyRenderObject()
	: RenderObject(Matrix44(true)) //identity
	{}

	const VertexData &GetVertexData() const
	{
		static const   VertexPNT square[4] = {
				{WorldPoint3(-0.5, -0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0) ,WorldPoint2(0.0, 0.0)} ,
				{WorldPoint3(0.5, -0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } ,
				{WorldPoint3(0.5, 0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } ,
				{WorldPoint3(-0.5, 0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } };
		static const VertexData data = {&square, 4, VertexPNT::GetComponentType()};
		return data;
	}
	void SetOrientation(Real x,Real y,Real z)
	{
		mTransform.SetRotationX(x);
		mTransform.SetRotationY(y);
		mTransform.SetRotationZ(z);
	}
	void SetPosition(const WorldPoint3 &position)
	{
		mTransform.SetPosition(position);
	}
	const Material &GetMaterial() const { return mMaterial; }
private:
	Material mMaterial;
};

void Test()
{
	TEST_START
	TextureResourceManager::Initialize();
	GLRenderer renderer;
	MyRenderObject obj;
	RenderObjectList objList;
	objList.push_back(&obj);

	const Matrix44 &transform = obj.GetTransform();
	std::stringstream s;
	s<<std::endl<<transform<<std::endl<<"=="<<std::endl<<Matrix44::IDENTITY;
	AssertMsg_Test(transform == Matrix44::IDENTITY, s.str().c_str());

	const WorldPoint3 position(0.0,0.0, -10.0);
	obj.SetPosition(position);
	Matrix44 positionTestMat(Matrix44::IDENTITY);
	positionTestMat.SetPosition(position);
	s.str("");
	s<<positionTestMat << "\n==\n" << obj.GetTransform() << std::endl;
	AssertMsg_Test(positionTestMat == obj.GetTransform(), s.str().c_str());


	renderer.Render(objList);

	TextureResourceManager::Terminate();


}
}
