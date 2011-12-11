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

#include <renderer/FrameBuffer.h>
#include <renderer/Material.h>
#include <renderer/RenderBuffer.h>
#include <renderer/RenderObject.h>
#include <renderer/Shader.h>
#include <renderer/Texture.h>
#include <renderer/TextureResourceManager.h>
#include <renderer/Vertex.h>

using namespace GCL;
namespace FrameBufferTest
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
				{WorldPoint3(-0.5, -0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0) ,WorldPoint2(0.0, 0.0)},
				{WorldPoint3(0.5, -0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 0.0)},
				{WorldPoint3(-0.5, 0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 1.0)},
				{WorldPoint3(0.5, 0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 1.0)}
		};
		static const VertexData data = {&square, 4, VertexPNT::GetComponentType()};
		return data;

	}
	const Material &GetMaterial() const { return mMaterial; }
private:
	Material mMaterial;


};

    bool CompareImages(const char * /*filename1*/, const char * /*filename2*/);
bool CompareImages(const char * /*filename1*/, const char * /*filename2*/)
{
	return false;
}
void Test();
void Test()
{
	TEST_START
	TextureResourceManager::Initialize();

	{
		GLRenderer renderer;
		size_t width = renderer.GetViewPort().GetWidth();
		size_t height = renderer.GetViewPort().GetHeight();

		RenderBuffer depthRenderBuffer(width, height);
		depthRenderBuffer.Bind();

		Texture texture(width, height, 4);
		texture.Bind();
		try
		{
			FrameBuffer frameBuffer(texture, depthRenderBuffer);
			frameBuffer.Bind();
			renderer.Render(RenderObjectList());
			FrameBuffer::ResetDefault();
		}
		catch (GCLException &e)
		{
			AssertMsg_Test(false, e.what());
		}

	}

	{
		GLRenderer renderer;

		Camera myCamera;

		MyRenderObject obj;
		obj.SetPosition(0,0,-10.0);
		RenderObjectList renderObjectList;
		renderObjectList.push_back(&obj);

		renderer.SetCamera(myCamera);
		size_t width = renderer.GetViewPort().GetWidth();
		size_t height = renderer.GetViewPort().GetHeight();

		RenderBuffer depthRenderBuffer(width, height);
		depthRenderBuffer.Bind();

		Texture texture(width, height, 4);

		try
		{
			FrameBuffer frameBuffer(texture, depthRenderBuffer);
			frameBuffer.Bind();
			renderer.Render(renderObjectList);
			FrameBuffer::ResetDefault();
		}
		catch (GCLException &e)
		{
			AssertMsg_Test(false, e.what());
		}
		texture.Save("FrameBufferTest.tga");
		//Assert_Test(CompareImages("RenderTargetTest.tga", "refRenderTargetTest.tga"));
	}

	TextureResourceManager::Terminate();
}
}
