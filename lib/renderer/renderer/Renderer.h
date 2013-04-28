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

#include "rendererconf.h"
#include GFXAPI_Renderer_H

namespace GCL
{
  class Renderer
  {
  public:
      Renderer(size_t windowsHandle)
  : mPimpl(windowsHandle)
      {
      }
    ~Renderer() {}
    bool Update() { return mPimpl.Update();}
	void PreRender() { mPimpl.PreRender(); }
	void PostRender() { mPimpl.PostRender(); }
    void Render(const RenderObjectList &renderObjectList) { mPimpl.Render(renderObjectList); }
    void Render(const RenderObject2DList &renderObjectList) { mPimpl.Render(renderObjectList); }
    void Render(const Text2DList &text2DList) { mPimpl.Render(text2DList); }
    void Render(uint8_t *rgb_front, uint8_t *depth_front) {mPimpl.Render(rgb_front, depth_front); }
    const ViewPort &GetViewPort() const { return mPimpl.GetViewPort(); }

	const std::string &GetVendor() const { return mPimpl.GetVendor() ; }
	const std::string &GetVersion() const { return mPimpl.GetVersion(); }
	const std::string &GetRenderer() const { return mPimpl.GetRenderer(); }
	const std::string &GetShadingLanguageVersion() const { return mPimpl.GetShadingLanguageVersion(); }
	const std::string &GetGlewVersion() const { return mPimpl.GetGlewVersion(); }
	const ExtensionList &GetExtensions() const { return mPimpl.GetExtensions(); }
	bool IsExtensionSupported(const std::string &ext) const { return mPimpl.IsExtensionSupported(ext); }

	void SetCamera(Camera &camera) { mPimpl.SetCamera(camera); }
	static void SetTransform( const Matrix44 &projection,
									const Matrix44 &modelView,
									const Matrix44 &transform,
									Shader *shader=NULL)
	{
		IRenderer::SetTransform(projection,modelView,transform,shader);
	}
	static Matrix44 GetGLProjection() {return IRenderer::GetGLProjection();}
	static Matrix44 GetGLModelView() {return IRenderer::GetGLModelView();}
  private:
      IRenderer mPimpl;
  };
}
