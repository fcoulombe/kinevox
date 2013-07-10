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

#include <kinetestlib/UnitTest.h>
#include <renderer/Renderer.h>
#include <windriver/WinDriver.h>

using namespace GCL;
namespace GLRendererTest
{
    
    void Test();
void Test()
{
	TEST_START
    WinDriver winDriver("GLRendererTest");
	{
	Renderer renderer(winDriver.GetWindowsHandle());

	std::cout << "OpenGL Stats"<<std::endl;
	std::cout << "Version: " << renderer.GetVersion()<<std::endl;
	std::cout << "Vendor: " << renderer.GetVendor()<<std::endl;
	std::cout << "Renderer: " << renderer.GetRenderer()<<std::endl;
	std::cout << "ShadingLanguageVersion: " << renderer.GetShadingLanguageVersion()<<std::endl;
	std::cout << "GlewVersion: " << renderer.GetGlewVersion()<<std::endl;

	std::cout << "Extensions: " << std::endl;
	const std::vector<std::string> &ext = renderer.GetExtensions();
	for (size_t i=0; i<ext.size();++i)
	{
		std::cout << ext[i] << std::endl;;
	}

#if  defined(ES2)
#ifdef OS_IPHONE
    Assert_Test(renderer.IsExtensionSupported("GL_OES_framebuffer_object"));
#endif
    Assert_Test(renderer.IsExtensionSupported("GL_OES_vertex_array_object"));
    Assert_Test(renderer.IsExtensionSupported("GL_IMG_texture_compression_pvrtc"));
#else
	Assert_Test(renderer.IsExtensionSupported("GL_ARB_multitexture"));
	Assert_Test(renderer.IsExtensionSupported("GL_ARB_pixel_buffer_object"));
	Assert_Test(renderer.IsExtensionSupported("GL_ARB_vertex_buffer_object"));
	Assert_Test(renderer.IsExtensionSupported("GL_ARB_vertex_program"));
	Assert_Test(renderer.IsExtensionSupported("GL_ARB_vertex_shader"));
    Assert_Test(renderer.IsExtensionSupported("GL_ARB_fragment_program"));
    Assert_Test(renderer.IsExtensionSupported("GL_ARB_fragment_shader"));
    Assert_Test(renderer.IsExtensionSupported("GL_ARB_shading_language_100"));
	Assert_Test(renderer.IsExtensionSupported("GL_ARB_texture_compression"));
    Assert_Test(renderer.IsExtensionSupported("GL_EXT_framebuffer_object"));
    // GL_EXT_texture_compression_s3tc      
#endif

	//test default viewport values
	{
		std::stringstream s;
		s<<renderer.GetViewPort().GetHeight()<<"=="<<Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT");
		AssertMsg_Test(renderer.GetViewPort().GetHeight()==(size_t)Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT"), s.str().c_str());
	}
	{
		std::stringstream s;
		s<<renderer.GetViewPort().GetWidth()<<"=="<<Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH");
		AssertMsg_Test(renderer.GetViewPort().GetWidth()==(size_t)Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH"), s.str().c_str());
	}

#if 0 //ENABLE_GLEW
	//Assert_Test(GLEW_ARB_vertex_program);
	Assert_Test(renderer.IsGlewExtensionSupported("GL_ARB_multitexture"));
	Assert_Test(renderer.IsGlewExtensionSupported("GL_ARB_pixel_buffer_object"));
	Assert_Test(renderer.IsGlewExtensionSupported("GL_ARB_vertex_buffer_object"));
	Assert_Test(renderer.IsGlewExtensionSupported("GL_ARB_vertex_program"));
	Assert_Test(renderer.IsGlewExtensionSupported("GL_ARB_vertex_shader"));
    Assert_Test(renderer.IsGlewExtensionSupported("GL_ARB_fragment_program"));
    Assert_Test(renderer.IsGlewExtensionSupported("GL_ARB_fragment_shader"));
    Assert_Test(renderer.IsGlewExtensionSupported("GL_ARB_shading_language_100"));
    Assert_Test(renderer.IsGlewExtensionSupported("GL_ARB_texture_compression"));
    Assert_Test(renderer.IsGlewExtensionSupported("GL_EXT_framebuffer_object"));
#endif
	}
}
}
