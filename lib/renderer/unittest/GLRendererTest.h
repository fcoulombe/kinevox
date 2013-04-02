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
#include <renderer/GLRenderer.h>
#include <windriver/WinDriver.h>

using namespace GCL;
namespace GLRendererTest
{
    
    void Test();
void Test()
{
	TEST_START
    WinDriver winDriver("GLRendererTest");
	GLRenderer renderer;


	std::cout << "OpenGL Stats"<<std::endl;
	std::cout << "Version: " << renderer.GetVersion()<<std::endl;
	std::cout << "Vendor: " << renderer.GetVendor()<<std::endl;
	std::cout << "Renderer: " << renderer.GetRenderer()<<std::endl;
	std::cout << "ShadingLanguageVersion: " << renderer.GetShadingLanguageVersion()<<std::endl;
	std::cout << "GlewVersion: " << renderer.GetGlewVersion()<<std::endl;

	/*std::cout << "Extensions: " << std::endl;
	const std::vector<std::string> &ext = renderer.GetExtensions();
	for (size_t i=0; i<ext.size();++i)
	{
		std::cout << ext[i] << std::endl;;
	}*/

#ifdef OS_IPHONE
#   ifdef ES1
    Assert_Test(renderer.IsExtensionSupported("GL_OES_framebuffer_object"));
    Assert_Test(renderer.IsExtensionSupported("GL_OES_vertex_array_object"));
    Assert_Test(renderer.IsExtensionSupported("GL_IMG_texture_compression_pvrtc"));
#   elif defined(ES2)
#       error "TBD: implement extension check for ES2"
#   else
#       error "INVALID"
#   endif
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
		s<<renderer.GetViewPort().GetHeight()<<"=="<<Config::Instance().GetInt("DEFAULT_SCREEN_HEIGHT");
		AssertMsg_Test(renderer.GetViewPort().GetHeight()==Config::Instance().GetInt("DEFAULT_SCREEN_HEIGHT"), s.str().c_str());
	}
	{
		std::stringstream s;
		s<<renderer.GetViewPort().GetWidth()<<"=="<<Config::Instance().GetInt("DEFAULT_SCREEN_WIDTH");
		AssertMsg_Test(renderer.GetViewPort().GetWidth()==Config::Instance().GetInt("DEFAULT_SCREEN_WIDTH"), s.str().c_str());
	}

	Matrix44 projection = GLRenderer::GetGLProjection();
	//here we simulate creating a modelviewmatrix of identity.
	//we modulate it with a transform
	//we perform the same operation in opengl
	//we make sure that our final modelview is equal to
	//the one of opengl. this asserts that we match in
	//matrix model
	{
		Matrix44 modelView = Inverse(Matrix44::IDENTITY);
		Matrix44 transform;
		transform.SetPosition(WorldPoint3(0.0,0.0,-10.0));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrix(reinterpret_cast<const GLreal*>(&transform));

		Matrix44 modelView1 = GLRenderer::GetGLModelView();
		GLRenderer::SetTransform(projection, modelView, transform);
		Matrix44 modelView2 = GLRenderer::GetGLModelView();

		std::stringstream s;
		s<<std::endl<<modelView1<<std::endl<<"=="<<std::endl<<modelView2;
		AssertMsg_Test(modelView1 == modelView2, s.str().c_str());

	}


#if ENABLE_GLEW
	Assert_Test(GLEW_ARB_vertex_program);
	Assert_Test(glewGetExtension("GL_ARB_multitexture"));
	Assert_Test(glewGetExtension("GL_ARB_pixel_buffer_object"));
	Assert_Test(glewGetExtension("GL_ARB_vertex_buffer_object"));
	Assert_Test(glewGetExtension("GL_ARB_vertex_program"));
	Assert_Test(glewGetExtension("GL_ARB_vertex_shader"));
    Assert_Test(glewGetExtension("GL_ARB_fragment_program"));
    Assert_Test(glewGetExtension("GL_ARB_fragment_shader"));
    Assert_Test(glewGetExtension("GL_ARB_shading_language_100"));
    Assert_Test(glewGetExtension("GL_ARB_texture_compression"));
    Assert_Test(glewGetExtension("GL_EXT_framebuffer_object"));
#endif
}
}
