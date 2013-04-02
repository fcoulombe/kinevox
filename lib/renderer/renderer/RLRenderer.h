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

#include <cstdlib>
#include <stdint.h>
#include <algorithm>
#include <string>

#include "renderer/Camera.h"
#include "renderer/Renderer.h"

#include <3rdparty/OpenRL.h>
namespace GCL
{

typedef std::vector<std::string> RLExtensionList;
class Shader;
class RLRenderer : public Renderer
{
public:
	RLRenderer();
	~RLRenderer();
	bool Update();
	void PreRender();
	void PostRender();
	virtual void Render(const RenderObjectList &renderObjectList);
	virtual void Render(const RenderObject2DList &spriteList, size_t width, size_t height);
	virtual void Render(const Text2DList &text2DList, size_t width, size_t height);
	void Render(uint8_t *rgb_front, uint8_t *depth_front);
	void RenderExtra(uint8_t *rgb_front, size_t width, size_t height, size_t depth);

	void SetCamera(Camera &camera) { mCamera = &camera; }

	struct RenderState
	{
		void SetTextureEnabled(bool isEnabled);
	};
	RenderState mCurrentRenderState;


	const std::string &GetVendor() const { return mVendor; }
	const std::string &GetVersion() const { return mVersion; }
	const std::string &GetRenderer() const { return mRenderer; }
	const std::string &GetShadingLanguageVersion() const { return mShadingLanguageVersion; }
	const RLExtensionList &GetExtensions() const { return mExtensions; }

	bool IsExtensionSupported(const std::string &ext) const
	{
		RLExtensionList::const_iterator b = mExtensions.begin();
		RLExtensionList::const_iterator e = mExtensions.end();
		RLExtensionList::const_iterator r =  std::find(b, e, ext);

		bool res = r != mExtensions.end();
		return res;
	}

	//the shader is when we want to set the project and
	//modelview*trasnform as uniform
	static void SetTransform( const Matrix44 &projection,
								const Matrix44 &modelView,
								const Matrix44 &transform,
								Shader *shader=NULL);

	static Matrix44 GetRLProjection();
	static Matrix44 GetRLModelView();

private:
	void Init3DState();

    OpenRLContext context;
	Camera *mCamera;

	std::string mVendor,
				mVersion,
				mRenderer,
				mShadingLanguageVersion;

	RLExtensionList mExtensions;


	uint32_t gl_depth_tex;
	uint32_t gl_rgb_tex;

    static void errorCatcher(RLenum /*error*/, const void* /*privateData*/,
        size_t /*privateSize*/, const char* message, void* /*userData*/)
    {
        GCLAssertMsg(false, message);
    }
};
}
