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
#include "applayer/GameStateManager.h"
#include "applayer/RenderTarget.h"
#include "applayer/Sprite.h"
#include "applayer/ScriptApi.h"
#include <gcl/Assert.h>
#include <gcl/BufferWriter.h>
#include <gcl/BufferReader.h>
#include <gcl/Log.h>
#include <gcl/Time.h>
#include <gcl/ThreadManager.h>
#include <input/Input.h>
#include <physics/PhysicsWorld.h>
#include <renderer/GeomUtilHelper.h>
#include <renderer/FrameBuffer.h>
#include <renderer/Renderer.h>
#include <renderer/RenderBuffer.h>
#include <renderer/FontResourceManager.h>
#include <renderer/MeshResourceManager.h>
#include <renderer/ShaderResourceManager.h>
#include <renderer/TextureResourceManager.h>
#include <script/ScriptResourceManager.h>
#include <sound/ALSoundDevice.h>
#include <sound/SoundResourceManager.h>
#include <sound/SoundManager.h>
#include <windriver/WinDriver.h>


//Components
#include "applayer/Component.h"
#include "applayer/RenderComponent.h"
#include "applayer/MeshComponent.h"
#include "applayer/RigidBodyComponent.h"
#include "applayer/SpriteComponent.h"
#include "applayer/ScriptComponent.h"

#define ENABLE_RENDER_TARGET 1
using namespace GCL;

GCLWorld *GCLApplication::mCurrentWorld= nullptr;
Renderer *GCLApplication::mRenderer = nullptr;
WinDriver *GCLApplication::mWinDriver = nullptr;
ScriptApi *GCLApplication::mScriptApi = nullptr;
RenderTarget *GCLApplication::mRenderTarget = nullptr;
ActorList GCLApplication::mActorList;
StrongActorList GCLApplication::mStrongActorList;
SpriteList GCLApplication::mSpriteList;

bool GCLApplication::mIsPaused = false;

class GameStates
{
public:
    std::string mWindowsTitle;
}gameStates;
namespace GCL
{
class GCLAppEventListener : public AppEventListener
{
public:
    GCLAppEventListener()
: mIsInFocus(true)
{}
    void OnGainFocus()
    {
        if (mIsInFocus)
            return;
        KLog("Gain focus");
        GCLApplication::InternalInitialize(gameStates.mWindowsTitle.c_str());
        GCLApplication::LoadStates();
        GCLApplication::SetPaused(false);
        mIsInFocus = true;
    }
    void OnLoseFocus()
    {
        if (!mIsInFocus)
            return;
        KLog("lose focus");
        GCLApplication::SaveStates();
        GCLApplication::SetPaused(true);
        // backup states.
        gameStates.mWindowsTitle = GCLApplication::GetWinDriver()->GetWindowsTitle();
        GCLApplication::InternalTerminate();
        mIsInFocus = false;
    }
private:
    bool mIsInFocus;
};
}
static GCLAppEventListener  *appEventListener = nullptr;

#define REGISTER_COMPONENT_FACTOR(name) \
        Component::Register(#name, \
                [](Actor *parentActor,PtrLuaTableIterator &it){return  std::pair<const char *, Component *>(#name, new name(parentActor, it));});

void GCLApplication::InitializaAppLayerComponents()
{
    REGISTER_COMPONENT_FACTOR(MeshComponent);
    REGISTER_COMPONENT_FACTOR(RenderComponent);
    REGISTER_COMPONENT_FACTOR(RigidBodyComponent);
    REGISTER_COMPONENT_FACTOR(SpriteComponent);
    REGISTER_COMPONENT_FACTOR(ScriptComponent);
}


/*static */void GCLApplication::Initialize(const char * windowsTitle)
{
    SoundResourceManager::Initialize();
    SoundManager::Initialize();
    ShaderResourceManager::Initialize();
    TextureResourceManager::Initialize();
    FontResourceManager::Initialize();
    ScriptResourceManager::Initialize();
    MeshResourceManager::Initialize();
    PhysicsWorld::Initialize();
    mScriptApi = new ScriptApi();
    InitializaAppLayerComponents();
    GameStateManager::Initialize();
    mWinDriver = new WinDriver(windowsTitle);

    RenderPipe::Initialize();
    InternalInitialize(windowsTitle);
    appEventListener = new GCLAppEventListener(); 

}
void GCLApplication::InternalInitialize(const char * /*windowsTitle*/)
{
    GCLAssert(mRenderer == NULL);

    mRenderer = new Renderer(mWinDriver->GetWindowsHandle());
    mRenderer->SetViewPort(
            ViewPort((size_t)0,
                    (size_t)0,
                    (size_t)Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH"),
                    (size_t)Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT")));
    const ViewPort &viewPort = mRenderer->GetViewPort();
    size_t width = viewPort.GetWidth();
    size_t height = viewPort.GetHeight();
    KLog("resolution: %dx%d", (int)width, (int)height);
#if ENABLE_RENDER_TARGET
    Point2<size_t> screenSize;
    mRenderer->GetScreenSize(screenSize);
    KLog("ScreenSize: %dx%d", (int)screenSize.x, (int)screenSize.y);
    mRenderTarget = new RenderTarget(width, height);
#endif

    mRenderer->SetVSyncEnabled(Config::Instance().GetBool("IS_VSYNC_ENABLED"));
}

void GCLApplication::InternalTerminate()
{
    mStrongActorList.clear();
    mActorList.clear();
#if ENABLE_RENDER_TARGET
    delete mRenderTarget;
    mRenderTarget = nullptr;
#endif
    GCLAssert(mRenderer);
    delete mRenderer;
    mRenderer = nullptr;
}
/*static */void GCLApplication::Terminate()
{
    GameStateManager::Terminate();
    delete appEventListener;
    appEventListener = nullptr;
    GCLAssertMsg(mStrongActorList.size() == 0, "You still have some strong actors hanging around");
    GCLAssertMsg(mActorList.size() == 0, "You still have some actors hanging around");

    InternalTerminate();

    RenderPipe::Terminate();
    Input::Terminate();
    GCLAssert(mWinDriver);
    delete mWinDriver;
    mWinDriver = nullptr;

    delete mScriptApi;


    PhysicsWorld::Terminate();
    MeshResourceManager::Terminate();
    ScriptResourceManager::Terminate();
    FontResourceManager::Terminate();
    TextureResourceManager::Terminate();
    ShaderResourceManager::Terminate();
    SoundManager::Terminate();
    SoundResourceManager::Terminate();
}


void GCLApplication::Update()
{
    size_t currentTime = GCL::Time::GetTickMs();
    static size_t lastTime = currentTime;
    Real mCurrentDt = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;
    if (mIsPaused)
    {
        Time::SleepMs(10);
        return;
    }

    SoundManager::Update();
    Input::ProcessInput();
    PhysicsWorld::Update(mCurrentDt);
    for (auto actor : mStrongActorList)
    {
        actor->Update(mCurrentDt);
    }
    GameStateManager::Update(mCurrentDt);
    ScriptResourceManager::Instance().Update();
    ThreadManager::ReThrowException();
}
void GCLApplication::Render()
{

    GCLAssert(mWinDriver);


    mWinDriver->SwapBuffer();
    if (mIsPaused)
    {
        return;
    }
    GCLAssert(mRenderer);
    //perform actor culling against view frustum

    //pass it to renderer
#if ENABLE_RENDER_TARGET
    mRenderTarget->Bind();
#endif
    mRenderer->PreRender();
    mRenderer->SetIsDepthMaskEnabled(true);
    mRenderer->SetIsDepthTestEnabled(true);
    mRenderer->SetIsBlendEnabled(true);
    //mRenderer->SetIsAlphaTestEnabled(true);

    Matrix44 proj;
    mRenderer->GetProjection(proj);
    mRenderer->SetIsDepthTestEnabled(true);
    std::sort(mActorList.begin(), mActorList.end(),
            [](const Actor * a, const Actor * b){return b->GetPosition().z < a->GetPosition().z;});
    for (size_t i=0; i<mActorList.size(); ++i)
    {
        mActorList[i]->Render(proj);
    }
    //mRenderer->Render(renderList);
    mRenderer->SetIsDepthMaskEnabled(false);
    mRenderer->SetIsDepthTestEnabled(false);
    mRenderer->SetIsBlendEnabled(true);
    //mRenderer->SetIsAlphaTestEnabled(false);
    mRenderer->SetOrtho();
    mRenderer->GetProjection(proj);
    std::sort(mSpriteList.begin(), mSpriteList.end(),
            [](const Sprite * a, const Sprite * b){return b->GetPosition().z > a->GetPosition().z;});
    for (size_t i=0; i<mSpriteList.size(); ++i)
    {
        Sprite *tempSprite =mSpriteList[i];
        tempSprite->Render(proj);
    }
    mRenderer->SetIsDepthTestEnabled(false);
    FrameBuffer::ResetDefault();
#if ENABLE_RENDER_TARGET
    const ViewPort &viewPort = mRenderer->GetViewPort();
    size_t width = viewPort.GetWidth();
    size_t height = viewPort.GetHeight();
    Point2<size_t> screenSize;
    mRenderer->GetScreenSize(screenSize);
    mRenderer->SetViewPort(
            ViewPort((size_t)0,(size_t)0,screenSize.x,screenSize.y));
    Matrix44 ortho;
    ortho.SetOrtho(-0.5, 0.5, 0.5,-0.5, -0.5, 0.5);
    mRenderTarget->Render(ortho);
    mRenderer->SetViewPort(
            ViewPort((size_t)0,
                    (size_t)0,
                    (size_t)width,
                    (size_t)height));
#endif
    mRenderer->PostRender();

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
    auto it = std::find_if(mActorList.begin(), mActorList.end(),
            [&](const Actor *actor) { return actor == &obj; });
    return it != mActorList.end();
}

void GCL::GCLApplication::RegisterWorld( GCLWorld* currentWorld )
{
    GCLAssert(mCurrentWorld == NULL); mCurrentWorld = currentWorld;
}

void GCL::GCLApplication::ReleaseWorld( GCLWorld* shouldBeTheSameWorldAsTheCurrent )
{
    GCLAssert(shouldBeTheSameWorldAsTheCurrent == mCurrentWorld); mCurrentWorld = NULL;
}

GCLEXPORT  Actor * GCL::GCLApplication::GetActor( const char *actorName )
{
    auto it = std::find_if(mActorList.begin(), mActorList.end(),
            [actorName](const Actor *actor) { return strcmp(actor->GetName().c_str(), actorName) == 0; });
    GCLAssertMsg(it != mActorList.end(), std::string("Couldn't find the actor: ") + actorName);
    return *it;
}

GCLEXPORT  ActorPtr GCL::GCLApplication::CreateActor( const char *actorName, const char *archetype )
{
    auto newActor = std::make_shared<Actor>(actorName, archetype);
    mStrongActorList.push_back(newActor);
    return newActor;
}

GCLEXPORT  void GCL::GCLApplication::DestroyActor( const char *actorName )
{
    auto it = std::find_if(mStrongActorList.begin(), mStrongActorList.end(),
            [actorName](const ActorPtr actor) { return strcmp(actor->GetName().c_str(), actorName) == 0; });
    GCLAssertMsg(it != mStrongActorList.end(), std::string("Couldn't find the actor: ") + actorName);
    mStrongActorList.erase(it);
}

GCLEXPORT  void GCL::GCLApplication::SaveStates()
{
    BufferWriter buffer(1024*100);
    buffer << mStrongActorList.size();
    for ( ActorPtr actor : mStrongActorList)
    {
        actor->SaveStates(buffer);
    }
    buffer.WriteToFile("GameStates.save");
}


GCLEXPORT  void GCL::GCLApplication::LoadStates()
{
    GCLFile saveStateFile("GameStates.save");
    BufferReader buffer(saveStateFile);
    size_t actorCount;
    buffer.Read(actorCount);
    for (size_t i=0; i<actorCount; ++i)
    {
        ActorPtr actor = std::make_shared<Actor>(buffer);
        mStrongActorList.push_back(actor);
    }
    for (ActorPtr actor : mStrongActorList)
    {
        actor->PatchReference(mStrongActorList);
    }
}

