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
#include <renderer/Shader.h>
#include <renderer/Vertex.h>

using namespace GCL;
namespace TextureAndShaderTest
{

class MyRenderObject : public RenderObject
{
public:
	MyRenderObject()
	: RenderObject(Matrix44(true)) //identity
	{}
	const VertexData &GetVertexData() const
	{
		return data;
	}
	const Material &GetMaterial() const { return mMaterial; }
private:
	Material mMaterial;
	static const VertexData data;

};

static const   VertexPNT square[4] = {
		{WorldPoint3(-0.5, -0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0) ,WorldPoint2(0.0, 0.0)},
		{WorldPoint3(0.5, -0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 0.0)},
		{WorldPoint3(-0.5, 0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 1.0)},
		{WorldPoint3(0.5, 0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 1.0)}
};

const VertexData MyRenderObject::data(&square, 4, VertexPNT::GetComponentType());

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
	GLRenderer renderer;
	Shader shader;
	shader.Bind();

	Texture texture(TEXTURE_PATH"mushroomtga.tga");
	texture.Bind();

	MyRenderObject obj;
	RenderObjectList renderObjectList;
	renderObjectList.push_back(&obj);
	renderer.Render(renderObjectList);


	        /*target.Save("TextureAndShaderTest.tga");
	        Assert_Test(CompareImages("RenderTargetTest.tga", "refRenderTargetTest.tga"));
	 */
	TextureResourceManager::Terminate();

}
}
