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

using namespace GCL;
namespace RenderObjectWithMaterialTest
{

class MyRenderObject : public RenderObject
{
public:
	MyRenderObject()
	: RenderObject(Matrix44::IDENTITY)
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
	void SetOrientation(WorldUnit x,WorldUnit y,WorldUnit z)
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


	{
	GLRenderer renderer;

	MyRenderObject obj;
	RenderObjectList objList;

	objList.push_back(&obj);

	Material material("Default");
	material.Bind();

	renderer.Render(objList);
	}

	TextureResourceManager::Terminate();
}
}
