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

#include "applayer/Actor.h"
#include "applayer/GCLText2D.h"
#include "applayer/Sprite.h"
#include <gcl/Assert.h>
#include <input/Input.h>
#include <renderer/Renderer.h>
#include <renderer/FontResourceManager.h>
#include <renderer/MeshResourceManager.h>
#include <renderer/TextureResourceManager.h>
#include <script/ScriptResourceManager.h>
#include <windriver/WinDriver.h>


//Components
#include "applayer/Component.h"
#include "applayer/RenderComponent.h"
#include "applayer/MeshComponent.h"
using namespace GCL;

GCLWorld *GCLApplication::mCurrentWorld=NULL;
Renderer *GCLApplication::mRenderer = NULL;
WinDriver *GCLApplication::mWinDriver = NULL;
ActorList GCLApplication::mActorList;
SpriteList GCLApplication::mSpriteList;


void GCLApplication::InitializaAppLayerComponents()
{
	Component::Register("RenderComponent", RenderComponent::Create);
	Component::Register("MeshComponent", MeshComponent::Create);
}
/*static */void GCLApplication::Initialize(const char *windowsTitle)
{
	TextureResourceManager::Initialize();
	FontResourceManager::Initialize();
	ScriptResourceManager::Initialize();
	MeshResourceManager::Initialize();
	GCLAssert(mRenderer == NULL);
    mWinDriver = new WinDriver(windowsTitle);
	mRenderer = new Renderer(mWinDriver->GetWindowsHandle());
	InitializaAppLayerComponents();
}
/*static */void GCLApplication::Terminate()
{
    GCLAssert(mActorList.size() == 0);
 	GCLAssert(mRenderer);
	delete mRenderer;
	mRenderer = NULL;
    GCLAssert(mWinDriver);
    delete mWinDriver;
    mWinDriver = NULL;
	MeshResourceManager::Terminate();
	ScriptResourceManager::Terminate();
	FontResourceManager::Terminate();
	TextureResourceManager::Terminate();
}


void GCLApplication::Update()
{
	Input::ProcessInput();
	//for (size_t i=0; i<m2DRenderObjectList.size(); ++i)
	//{
	//	m2DRenderObjectList[i]->Update();
//	}

}
void GCLApplication::Render()
{
    GCLAssert(mWinDriver);
	GCLAssert(mRenderer);
	//perform actor culling against view frustom

	//pass it to renderer
	mRenderer->PreRender();
	for (size_t i=0; i<mActorList.size(); ++i)
	{
		mActorList[i]->Render();
	}
	//mRenderer->Render(renderList);
	mRenderer->SetOrtho();
	for (size_t i=0; i<mSpriteList.size(); ++i)
	{
		Sprite *tempSprite =mSpriteList[i];
		tempSprite->Render();
	}
	mRenderer->PostRender();
	mWinDriver->SwapBuffer();
}

void GCLApplication::SetViewportCamera(Camera &camera)
{
	mRenderer->SetCamera(camera);
}

void GCLApplication::RegisterCustomRenderObject(Actor* newRenderObj)
{
	GCLAssert(newRenderObj);
	mActorList.push_back(newRenderObj);
}

void GCLApplication::RegisterRenderObject(Actor* newRenderObj)
{
	GCLAssert(newRenderObj);
	mActorList.push_back(newRenderObj);
}

void GCLApplication::ReleaseRenderObject(Actor* renderObjToDelete)
{
	GCLAssert(renderObjToDelete);
	auto it = std::find(mActorList.begin(), mActorList.end(), renderObjToDelete);
	GCLAssert(it != mActorList.end());
	mActorList.erase(it);
}
void GCLApplication::ReleaseSprite(Sprite* renderObjToDelete)
{
	GCLAssert(renderObjToDelete);
	auto it = std::find(mSpriteList.begin(), mSpriteList.end(), renderObjToDelete);
	GCLAssert(it != mSpriteList.end());
	mSpriteList.erase(it);
}
#if 0
void GCLApplication::ReleaseText2D(GCLText2D* textToDelete)
{
    GCLAssert(textToDelete);
	Text2DList::iterator it = std::find(mText2DList.begin(),
			mText2DList.end(),
			textToDelete);
	GCLAssert(it != mText2DList.end());
	mText2DList.erase(it);
}
#endif

bool GCLApplication::IsRegistered(const Actor &obj)
{
	for (auto it = mActorList.begin(); it !=mActorList.end(); ++it)
	{
		const Actor* ro = *it;
		if (ro == &obj)
			return true;
	}
	return false;
}

void GCL::GCLApplication::RegisterWorld( GCLWorld* currentWorld )
{
    GCLAssert(mCurrentWorld == NULL); mCurrentWorld = currentWorld;
}

void GCL::GCLApplication::ReleaseWorld( GCLWorld* shouldBeTheSameWorldAsTheCurrent )
{
    GCLAssert(shouldBeTheSameWorldAsTheCurrent == mCurrentWorld); mCurrentWorld = NULL;
}
