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
    static const   VertexPNT square[4] = {
        {WorldPoint3(-0.5, -0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0) ,WorldPoint2(0.0, 0.0)} ,
        {WorldPoint3(0.5, -0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } ,
        {WorldPoint3(0.5, 0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } ,
        {WorldPoint3(-0.5, 0.5, 0.0), WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 0.0) } };
class MyRenderObject : public RenderObject
{
public:
	MyRenderObject()
	: RenderObject("MyRenderObject", Matrix44(true)) //identity
	{
        data.push_back(VertexData(&square, 4, VertexPNT::GetComponentType()));
    }

	const VertexDataList &GetVertexData() const
	{
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
	VertexDataList data;
	Material mMaterial;
};

void Test();
void Test()
{
	TEST_START

	TextureResourceManager::Initialize();
	{
		WinDriver winDriver("RenderObjectWithMaterialTest");
		GLRenderer renderer;

		MyRenderObject obj;
		RenderObjectList objList;

		objList.push_back(&obj);

		Material material("Default");
		material.Bind();

		renderer.PreRender();
		renderer.Render(objList);
		renderer.PostRender();
		winDriver.SwapBuffer();
	}
	TextureResourceManager::Terminate();
}
}
