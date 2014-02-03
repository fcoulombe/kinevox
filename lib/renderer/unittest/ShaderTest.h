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
#include <gcl/Time.h>
#include <kinetestlib/UnitTest.h>
#include <renderer/GeomUtilHelper.h>
#include <renderer/Shader.h>
#include <renderer/GPUProgram.h>
#include <renderer/Texture.h>
#include <renderer/ShaderAttributeLocations.h>
#include <renderer/Material.h>
#include <renderer/TextureResourceManager.h>


using namespace GCL;
namespace ShaderTest
{

void Test();
void Test()
{
	KINEVOX_TEST_START
		ShaderResourceManager::Initialize();
	TextureResourceManager::Initialize();
	{
		WinDriver winDriver("ShaderTest");
		Renderer renderer(winDriver.GetWindowsHandle());
		GPUProgram program;
		Shader vertexShader("DefaultVertexShader", VERTEX_SHADER);
		Shader fragmentShader("DefaultFragmentShader", FRAGMENT_SHADER);
		program.AttachShader(vertexShader);
		program.AttachShader(fragmentShader);
		program.Link();
		program.Bind();

		Assert_Test(program.IsValid());

		//set custom uniform
		int testValue=56;
		program.SetUniform("TestUniform", testValue);
		program.GetUniform("TestUniform", testValue);
		Assert_Test(testValue == 56);

		//set matrix uniform test
		Camera cam;
		cam.Update();

		Matrix44 modelView;
		modelView.SetRotationX(90.0);
		program.SetModelViewMatrix(modelView);
		program.SetProjectionMatrix();

		//query uniform fail test

		std::stringstream s;

		//query pro0jection matrix test
		Matrix44 proj2;
		program.GetUniform("ProjectionMatrix", proj2);
		s.str("");
		s<<std::endl<<proj2<<std::endl<<"=="<<std::endl<<renderer.GetProjection();
		AssertMsg_Test(proj2==renderer.GetProjection(), s.str().c_str());

		//query modelview matrix test
		Matrix44 modelView2;
		program.GetUniform("ModelViewMatrix", modelView2);
		s.str("");
		s<<std::endl<<modelView2<<std::endl<<"=="<<std::endl<<modelView;
		AssertMsg_Test(modelView2==modelView, s.str().c_str());


		Texture tex(TEXTURE_PATH"mushroomtga.tga");
		tex.Bind();
		program.SetTextureSampler(tex);

		int sampler;
		program.GetUniform("texture", sampler);
		s.str("");
		s<<sampler<<" == 0";
		AssertMsg_Test(sampler==0, s.str().c_str());


		//attribute
		enum AttributePositions
		{
		  ATTRIB_POSITION=0,
		  ATTRIB_NORMAL=2,
		  ATTRIB_TEXTURE_COORD=1
		};
		//attribute position query test
		int loc = program.GetAttributeLocation("InPosition");
		s.str("");
		s<<loc<<" != -1";
		AssertMsg_Test(loc != -1, s.str().c_str());
#ifndef ES2
        //attribute normal query test
        loc = program.GetAttributeLocation("InNormal");
        s.str("");
        s<<loc<<" != -1";
        AssertMsg_Test(loc != -1, s.str().c_str());
#endif

		//attribute texcoord query test
		loc = program.GetAttributeLocation("InTexCoord");
		s.str("");
		s<<loc<<" != -1";
		AssertMsg_Test(loc != -1, s.str().c_str());

		renderer.PreRender();
		//renderer.Render(RenderObjectList());
		renderer.PostRender();
		GPUProgram::ResetDefault();
	}

	{
		WinDriver winDriver("ShaderTest");
		Renderer renderer(winDriver.GetWindowsHandle());
		SquareRenderObject obj;
		const WorldPoint3 position(0.0,0.0, -10.0);
		obj.SetPosition(position);

		KINEVOX_TEST_LOOP_START
			renderer.PreRender();
			obj.Render();
			renderer.PostRender();
			winDriver.SwapBuffer();
		KINEVOX_TEST_LOOP_END

		GPUProgram::ResetDefault();
	}
	TextureResourceManager::Terminate();	
	ShaderResourceManager::Terminate();
}
}
