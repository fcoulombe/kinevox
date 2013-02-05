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
#include <renderer/Texture.h>
#include <renderer/Vertex.h>

using namespace GCL;
namespace TextureAndShaderTest
{
    static const   VertexPNT square[4] = {
        {WorldPoint3(-0.5, -0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0) ,WorldPoint2(0.0, 0.0)},
        {WorldPoint3(0.5, -0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 0.0)},
        {WorldPoint3(-0.5, 0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 1.0)},
        {WorldPoint3(0.5, 0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 1.0)}
    };
class MyRenderObject : public RenderObject
{
public:
	MyRenderObject()
	: RenderObject("MyRenderObject", Matrix44(true)) //identity
	{}
	~MyRenderObject()
	{
        data.push_back(VertexData(&square, 4, VertexPNT::GetComponentType()));
	}
	const VertexDataList &GetVertexData() const
	{
		return data;
	}
	const Material &GetMaterial() const { return mMaterial; }
	Material &GetMaterial() { return mMaterial; }
private:
	Material mMaterial;
	VertexDataList data;
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
        WinDriver winDriver("TextureAndShaderTest");
        GLRenderer renderer;
		Shader shader;
		shader.Bind();

		MyRenderObject obj1;
		const WorldPoint3 position1(2.5,0.0, -10.0);
		obj1.SetPosition(position1);
		obj1.GetMaterial().SetTexture(TEXTURE_PATH"mushroomcompressedtga.tga");


		MyRenderObject obj2;
		const WorldPoint3 position2(-2.5,0.0, -10.0);
		obj2.SetPosition(position2);
		obj2.GetMaterial().SetTexture(TEXTURE_PATH"HappyFish.tga");

		RenderObjectList renderObjectList;
		renderObjectList.push_back(&obj1);

		renderObjectList.push_back(&obj2);
		renderer.PreRender();
		renderer.Render(renderObjectList);
		renderer.PostRender();
        winDriver.SwapBuffer();

		/*target.Save("TextureAndShaderTest.tga");
	        Assert_Test(CompareImages("RenderTargetTest.tga", "refRenderTargetTest.tga"));
		 */
	}
	TextureResourceManager::Terminate();
}
}
