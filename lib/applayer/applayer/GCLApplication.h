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
#include <windriver/WinDriver.h>
namespace GCL
{
class Actor;
class Camera;
class GCLText2D;
class GCLWorld;
class Input;
class Renderer;
class RenderTarget;
class ScriptApi;
class Sprite;
class Text2D;
class WinDriver;
typedef std::vector<Actor *> ActorList;
typedef std::shared_ptr<Actor> ActorPtr;
typedef std::vector<ActorPtr> StrongActorList;

typedef std::vector<Sprite *> SpriteList;
class GCLApplication
{
public:
	GCLEXPORT static void Initialize(const char *windowsTitle);
	GCLEXPORT static void Terminate();

	GCLEXPORT static void Update();
	GCLEXPORT static void Render();

	GCLEXPORT static void SetViewportCamera(Camera &camera);

	GCLTEST_EXPORT static bool IsRegistered(const Actor &obj);

	GCLEXPORT static Renderer *GetRenderer() { return mRenderer; }
    GCLEXPORT static WinDriver *GetWinDriver() { return mWinDriver; }
	GCLEXPORT static void RegisterCustomRenderObject(Actor* newRenderObject);

	GCLEXPORT static void InitializaAppLayerComponents();
    GCLEXPORT static Actor *GetActor(const char *actorName);

    GCLEXPORT static ActorPtr CreateActor(const char *actorName, const char *archetype);
    GCLEXPORT static void DestroyActor(const char *actorName);

    GCLEXPORT static Real GetCurrentDt() { return mCurrentDt; }
private:
	GCLApplication();
	~GCLApplication();

	friend class Actor;
	friend class GCLRenderObject2D;
	friend class GCLText2D;
    friend class GCLWorld;
	friend class Sprite;
	static void RegisterRenderObject(Actor* newRenderObject);
	static void ReleaseRenderObject(Actor* renderObjectToDelete);

	static void RegisterSprite(Sprite* newRenderObject) { mSpriteList.push_back(newRenderObject); }
	static void ReleaseSprite(Sprite* renderObjectToDelete);

	static void RegisterText2D(GCLText2D* newRenderText2d);
	static void ReleaseText2D(GCLText2D* renderText2DToDelete);

    static void RegisterWorld(GCLWorld* currentWorld);
    static void ReleaseWorld(GCLWorld* shouldBeTheSameWorldAsTheCurrent);

	static Renderer *mRenderer;
    static WinDriver *mWinDriver;

    static GCLWorld *mCurrentWorld;
	static ActorList mActorList;
    static StrongActorList mStrongActorList;
	static SpriteList mSpriteList;

	static ScriptApi *mScriptApi;
	static RenderTarget *mRenderTarget;

	static Real mCurrentDt;

};

}
