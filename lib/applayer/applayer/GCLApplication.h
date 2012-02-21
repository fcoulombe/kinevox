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

#include <vector>
#include <gcl/GCLExport.h>
#include <renderer/Renderer.h>

namespace GCL
{
class Camera;
class GLRenderer;
class GCLRenderObject;
class GCLSprite;
class Input;

class GCLApplication
{
public:
	GCLEXPORT static void Initialize();
	GCLEXPORT static void Terminate();

	GCLEXPORT static void Update();
	GCLEXPORT static void Render();

	GCLEXPORT static void SetViewportCamera(Camera &camera);


	GCLTEST_EXPORT static bool IsRegistered(const GCLRenderObject &obj);

	GCLEXPORT static GLRenderer *GetRenderer() { return mRenderer; }
private:
	GCLApplication();
	~GCLApplication();

	friend class 	GCLRenderObject;
	friend class GCLSprite;
	static void RegisterRenderObject(GCLRenderObject* newRenderObject);
	static void ReleaseRenderObject(GCLRenderObject* renderObjectToDelete);

	static void RegisterSprite(GCLSprite* newSpriteObject);
	static void ReleaseSprite(GCLSprite* spriteToDelete);


	static GLRenderer *mRenderer;

	static RenderObjectList mRenderObjectList;
	static SpriteList mSpriteList;
};

}
