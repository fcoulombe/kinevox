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
#include <renderer/Shader.h>
#include <renderer/ShaderAttributeDefaultLocations.h>


using namespace GCL;
namespace ShaderTest
{

class MyRenderObject : public RenderObject
{
public:
	MyRenderObject()
	: RenderObject("MyRenderObject", Matrix44(true)) //identity
	{}
	const VertexData &GetVertexData() const
	{
		return data;
	}
	const Material &GetMaterial() const { return mMaterial; }
private:
	static const VertexData data;
	Material mMaterial;
};
static const   VertexPNT square[4] = {
				{WorldPoint3(-0.5, -0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0) ,WorldPoint2(0.0, 0.0)},
				{WorldPoint3(0.5, -0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 0.0)},
				{WorldPoint3(-0.5, 0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(0.0, 1.0)},
				{WorldPoint3(0.5, 0.5, 0.0), 	WorldPoint3(0.0, 0.0, 1.0), WorldPoint2(1.0, 1.0)}
		};
const VertexData MyRenderObject::data(&square, 4, VertexPNT::GetComponentType());
    
    void Test();
void Test()
{
	TEST_START
    #ifndef ENABLE_SHADERS
	TextureResourceManager::Initialize();
	{
        WinDriver winDriver;
        GLRenderer renderer;
		Shader shader;
		shader.Bind();
		Assert_Test(shader.IsValid());

		//set matrix uniform test
		Matrix44 proj;
		proj.SetPerspective(45.0, 640.0/480.0,0.1,100.0);
		Matrix44 modelView;
		modelView.SetRotationX(90.0);
		shader.SetModelViewMatrix(modelView);
		shader.SetProjectionMatrix(proj);

		//query uniform fail test
		try
		{
			Matrix44 m2;
			shader.GetUniform("NonExistingUniform", m2);
			Assert_Test(true);
		}
		catch (GCLException &e)
		{
		}

		std::stringstream s;
#if !ENABLE_FIX_PIPELINE

		//query pro0jection matrix test
		Matrix44 proj2;
		shader.GetUniform("ProjectionMatrix", proj2);
		s.str("");
		s<<std::endl<<proj2<<std::endl<<"=="<<std::endl<<proj;
		AssertMsg_Test(proj2==proj, s.str().c_str());

		//query modelview matrix test
		Matrix44 modelView2;
		shader.GetUniform("ModelViewMatrix", modelView2);
		s.str("");
		s<<std::endl<<modelView2<<std::endl<<"=="<<std::endl<<modelView;
		AssertMsg_Test(modelView2==modelView, s.str().c_str());
#endif


		Texture tex(TEXTURE_PATH"mushroomtga.tga");
		tex.Bind();
		shader.SetTextureSampler(tex);

		int sampler;
		shader.GetUniform("texture", sampler);
		s.str("");
		s<<sampler<<" == 0";
		AssertMsg_Test(sampler==0, s.str().c_str());


		//attribute
		//attribute location fail test
		try
		{
			shader.GetAttributeLocation("NonExistingAttribute");
			Assert_Test(true);
		}
		catch (GCLException & /*e*/)
		{}

#if !ENABLE_FIX_PIPELINE
		//attribute position query test
		int loc = shader.GetAttributeLocation("InPosition");
		s.str("");
		s<<loc<<" == ATTRIB_POSITION";
		AssertMsg_Test(loc == ATTRIB_POSITION, s.str().c_str());

		//attribute texcoord query test
		loc = shader.GetAttributeLocation("InTexCoord");
		s.str("");
		s<<loc<<" == ATTRIB_TEXTURE_COORD";
		AssertMsg_Test(loc == ATTRIB_TEXTURE_COORD, s.str().c_str());

		//attribute normal query test
		loc = shader.GetAttributeLocation("InNormal");
		s.str("");
		s<<loc<<" == ATTRIB_NORMAL";
		AssertMsg_Test(loc == ATTRIB_NORMAL, s.str().c_str());
#endif

		renderer.PreRender();
		renderer.Render(RenderObjectList());
		renderer.PostRender();
        winDriver.SwapBuffer();
		Shader::ResetDefault();
	}

	{
        WinDriver winDriver;
		GLRenderer renderer;
		MyRenderObject obj;
		const WorldPoint3 position(0.0,0.0, -10.0);
		obj.SetPosition(position);

		RenderObjectList renderList;
		renderList.push_back(&obj);

		for (size_t i=0; i<100; ++i)
		{
			renderer.PreRender();
			renderer.Render(renderList);
			renderer.PostRender();
            winDriver.SwapBuffer();
			Time::SleepMs(10);
		}

		Shader::ResetDefault();
	}

	/*VertexPNT square[4] = { {WorldPoint3(-0.5, -0.5, 0.0), WorldPoint2(0.0, 0.0), WorldPoint3(0.0, 0.0, 1.0) } ,
                          {WorldPoint3(0.5, -0.5, 0.0), WorldPoint2(0.0, 0.0), WorldPoint3(0.0, 0.0, 1.0) } ,
                          {WorldPoint3(0.5, 0.5, 0.0), WorldPoint2(0.0, 0.0), WorldPoint3(0.0, 0.0, 1.0) } ,
                          {WorldPoint3(-0.5, 0.5, 0.0), WorldPoint2(0.0, 0.0), WorldPoint3(0.0, 0.0, 1.0) } };
  VertexBuffer<VertexPNT> vb(square, 4);

  renderer->Render(vb);
	 */

	TextureResourceManager::Terminate();
#endif
}
}
