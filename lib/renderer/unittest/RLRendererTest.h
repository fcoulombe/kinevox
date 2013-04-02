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
#include <windriver/WinDriver.h>

using namespace GCL;
namespace RLRendererTest
{
    
    void Test();
void Test()
{
	TEST_START
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
		s<<winDriver.GetViewPort().GetHeight()<<"=="<<ViewPort::DEFAULT_SCREEN_HEIGHT;
		AssertMsg_Test(winDriver.GetViewPort().GetHeight()==ViewPort::DEFAULT_SCREEN_HEIGHT, s.str().c_str());
	}
	{
		std::stringstream s;
		s<<winDriver.GetViewPort().GetWidth()<<"=="<<ViewPort::DEFAULT_SCREEN_WIDTH;
		AssertMsg_Test(winDriver.GetViewPort().GetWidth()==ViewPort::DEFAULT_SCREEN_WIDTH, s.str().c_str());
	}

    RLTexture colorBuffer(winDriver.GetViewPort().GetWidth(),winDriver.GetViewPort().GetHeight());
    RLFrameBuffer frameBuffer(colorBuffer);


}
}
