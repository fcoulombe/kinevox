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
#pragma once

#include <kinetestlib/UnitTest.h>
#include <renderer/RLRenderer.h>
#include <renderer/RLTexture.h>
#include <renderer/RLFrameBuffer.h>
#include <renderer/RLProgram.h>
#include <renderer/RLShader.h>
#include <renderer/RLVertexBuffer.h>
#include <windriver/WinDriver.h>
#include <renderer/TextureResourceManager.h>
#include <renderer/GeomUtil.h>
using namespace GCL;
namespace RLRendererTest
{
    
    void Test();
void Test()
{
	TEST_START
        	TextureResourceManager::Initialize();
    {
    WinDriver winDriver("GLRendererTest");
	RLRenderer renderer;


	std::cout << "OpenGL Stats"<<std::endl;
	std::cout << "Version: " << renderer.GetVersion()<<std::endl;
	std::cout << "Vendor: " << renderer.GetVendor()<<std::endl;
	std::cout << "Renderer: " << renderer.GetRenderer()<<std::endl;
	std::cout << "ShadingLanguageVersion: " << renderer.GetShadingLanguageVersion()<<std::endl;

	std::cout << "Extensions: " << std::endl;
	const std::vector<std::string> &ext = renderer.GetExtensions();
	for (size_t i=0; i<ext.size();++i)
	{
		std::cout << ext[i] << std::endl;;
	}


	//test default viewport values
	{
		std::stringstream s;
		s<<renderer.GetViewPort().GetHeight()<<"=="<<Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT");
		AssertMsg_Test(renderer.GetViewPort().GetHeight()==Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT"), s.str().c_str());
	}
	{
		std::stringstream s;
		s<<renderer.GetViewPort().GetWidth()<<"=="<<Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH");
		AssertMsg_Test(renderer.GetViewPort().GetWidth()==Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH"), s.str().c_str());
	}

    RLTexture mush(TEXTURE_PATH"mushroomtga.tga");
    mush.Save("RLMushroom.tga");

    RLTexture colorBuffer(renderer.GetViewPort().GetWidth(),renderer.GetViewPort().GetHeight());
    RLFrameBuffer frameBuffer(colorBuffer);
    RLProgram program;
    RLShader frameShader("DefaultFrameShader.txt", RL_FRAME_SHADER);
    RLShader vertexShader("DefaultVertexShader.txt", RL_VERTEX_SHADER);
    RLShader rayShader("DefaultRayShader.txt", RL_RAY_SHADER);
    program.AttachShader(frameShader);
    //program.AttachShader(vertexShader);
    //program.AttachShader(rayShader);
    program.Link();
    program.Bind();
    AttribLocations loc;
    loc.position = program.GetAttributeLocation("InPosition");

    std::vector<WorldPoint3> vertexData;
    std::vector<WorldPoint3> normalData;
    std::vector<WorldPoint2> tcoordData;
    GeomUtil::MakeMeshCube(vertexData,normalData,tcoordData,1.0);
 
    RLVertexBuffer<VertexP> buffer((const VertexP*)vertexData.data(), vertexData.size());
    buffer.BindShaderLoc(loc);
    buffer.Render();
    
    float transformMatrix[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, -3.0f, 1.0f,
    };
    rlPrimitiveParameterMatrixf(RL_PRIMITIVE, RL_PRIMITIVE_TRANSFORM_MATRIX, transformMatrix);
    rlRenderFrame();

    colorBuffer.Save("RLColorBuffer.tga");
    }
    	TextureResourceManager::Terminate();

}
}
