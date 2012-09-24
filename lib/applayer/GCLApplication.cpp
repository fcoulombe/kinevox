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

#include "applayer/GCLApplication.h"

#include <algorithm>

#include "applayer/GCLRenderObject.h"
#include "applayer/GCLRenderObject2D.h"
#include "applayer/GCLText2D.h"
#include <gcl/Assert.h>
#include <input/Input.h>
#include <renderer/GLRenderer.h>
#include <renderer/MeshResourceManager.h>
#include <renderer/TextureResourceManager.h>
#include <windriver/FontResourceManager.h>
#include <windriver/WinDriver.h>

using namespace GCL;

GLRenderer *GCLApplication::mRenderer = NULL;
WinDriver *GCLApplication::mWinDriver = NULL;
RenderObjectList GCLApplication::mRenderObjectList;
RenderObject2DList GCLApplication::mRenderObject2DList;
Text2DList GCLApplication::mText2DList;


/*static */void GCLApplication::Initialize()
{
	TextureResourceManager::Initialize();
	FontResourceManager::Initialize();
	MeshResourceManager::Initialize();
	GCLAssert(mRenderer == NULL);
    mWinDriver = new WinDriver();
	mRenderer = new GLRenderer();
}
/*static */void GCLApplication::Terminate()
{
	GCLAssert(mRenderer);
	delete mRenderer;
	mRenderer = NULL;
    GCLAssert(mWinDriver);
    delete mWinDriver;
    mWinDriver = NULL;
	MeshResourceManager::Terminate();
	FontResourceManager::Terminate();
	TextureResourceManager::Terminate();
}


void GCLApplication::Update()
{
	Input::ProcessInput();
	for (size_t i=0; i<mRenderObject2DList.size(); ++i)
	{
		mRenderObject2DList[i]->Update();
	}
}
void GCLApplication::Render()
{
    GCLAssert(mWinDriver);
	GCLAssert(mRenderer);
	mRenderer->PreRender();
	mRenderer->Render(mRenderObjectList);
    const ViewPort &tempViewPort = mWinDriver->GetViewPort();
	mRenderer->Render(mRenderObject2DList, tempViewPort.GetWidth(), tempViewPort.GetHeight());
	mRenderer->Render(mText2DList, tempViewPort.GetWidth(), tempViewPort.GetHeight());
	mRenderer->PostRender();
    mWinDriver->SwapBuffer();
}

void GCLApplication::SetViewportCamera(Camera &camera)
{
	mRenderer->SetCamera(camera);
}

void GCLApplication::RegisterCustomRenderObject(RenderObject* newRenderObj)
{
	GCLAssert(newRenderObj);
	mRenderObjectList.push_back(newRenderObj);
}

void GCLApplication::RegisterRenderObject(GCLRenderObject* newRenderObj)
{
	GCLAssert(newRenderObj);
	mRenderObjectList.push_back(newRenderObj);
}

void GCLApplication::ReleaseRenderObject(GCLRenderObject* renderObjToDelete)
{
	GCLAssert(renderObjToDelete);
	RenderObjectList::iterator it = std::find(mRenderObjectList.begin(), mRenderObjectList.end(), renderObjToDelete);
	GCLAssert(it != mRenderObjectList.end());
	mRenderObjectList.erase(it);
}

void GCLApplication::RegisterRenderObject2D(GCLRenderObject2D* newRenderObject)
{
	GCLAssert(newRenderObject);
	mRenderObject2DList.push_back(newRenderObject);
}

void GCLApplication::ReleaseRenderObject2D(GCLRenderObject2D* renderObjectToDelete)
{
	GCLAssert(renderObjectToDelete);
	RenderObject2DList::iterator it = std::find(mRenderObject2DList.begin(),
												mRenderObject2DList.end(),
												renderObjectToDelete);
	GCLAssert(it != mRenderObject2DList.end());
	mRenderObject2DList.erase(it);
}

void GCLApplication::RegisterText2D(GCLText2D* newText2D)
{
	GCLAssert(newText2D);
	mText2DList.push_back(newText2D);
}

void GCLApplication::ReleaseText2D(GCLText2D* textToDelete)
{
	GCLAssert(textToDelete);
/*	Text2DList::iterator it = std::find(mText2DList.begin(),
			mText2DList.end(),
			mText2DList);
	GCLAssert(it != mText2DList.end());
	mText2DList.erase(it);*/
}


bool GCLApplication::IsRegistered(const GCLRenderObject &obj)
{
	for (RenderObjectList::const_iterator it = mRenderObjectList.begin(); it !=mRenderObjectList.end(); ++it)
	{
		const RenderObject* ro = *it;
		if (ro == &obj)
			return true;
	}
	return false;
}
