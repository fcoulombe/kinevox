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

#include <algorithm>
#include <cstdlib>
#include <stdint.h>
#include <string>
#include <vector>

#include <gcl/Config.h>
#include "renderer/Camera.h"
#include "renderer/ViewPort.h"

namespace GCL
{
    class RenderObject;
    class RenderObject2D;
    class Text2D;
    typedef std::vector<const RenderObject*> RenderObjectList;
    typedef std::vector<RenderObject2D*> RenderObject2DList;
    typedef std::vector<Text2D*> Text2DList;
typedef std::vector<std::string> ExtensionList;
class Shader;
class GLRenderer
{
public:
	GLRenderer(size_t windowsHandle);
	~GLRenderer();
	bool Update();
	void PreRender();
	void PostRender();
	void Render(const RenderObjectList &renderObjectList);
	void Render(const RenderObject2DList &spriteList);
	void Render(const Text2DList &text2DList);

	void SetCamera(Camera &camera) { mCamera = &camera; }

	struct RenderState
	{
		void SetTextureEnabled(bool isEnabled);
	};
	RenderState mCurrentRenderState;

    const ViewPort &GetViewPort() const { return mViewPort; }
	const std::string &GetVendor() const { return mVendor; }
	const std::string &GetVersion() const { return mVersion; }
	const std::string &GetRenderer() const { return mRenderer; }
	const std::string &GetShadingLanguageVersion() const { return mShadingLanguageVersion; }
	const std::string &GetGlewVersion() const { return mGlewVersion; }
	const ExtensionList &GetExtensions() const { return mExtensions; }

	bool IsExtensionSupported(const std::string &ext) const
	{
		ExtensionList::const_iterator b = mExtensions.begin();
		ExtensionList::const_iterator e = mExtensions.end();
		ExtensionList::const_iterator r =  std::find(b, e, ext);

		bool res = r != mExtensions.end();
		return res;
	}

	//the shader is when we want to set the project and
	//modelview*trasnform as uniform
	static void SetTransform( const Matrix44 &projection,
								const Matrix44 &modelView,
								const Matrix44 &transform,
								Shader *shader=NULL);

	static Matrix44 GetGLProjection();
	static Matrix44 GetGLModelView();

private:
	void Init3DState();
    ViewPort mViewPort;
	Camera *mCamera;

	std::string mVendor,
				mVersion,
				mRenderer,
				mShadingLanguageVersion,
				mGlewVersion;

	ExtensionList mExtensions;


};
}
