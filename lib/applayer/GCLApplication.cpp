#include "applayer/GCLApplication.h"

#include <algorithm>

#include "applayer/GCLRenderObject.h"
#include <gcl/Assert.h>
#include <input/Input.h>
#include <renderer/GLRenderer.h>
#include <renderer/TextureResourceManager.h>

using namespace GCL;

GLRenderer *GCLApplication::mRenderer = NULL;
RenderObjectList GCLApplication::mRenderObjectList;


/*static */void GCLApplication::Initialize()
{
	TextureResourceManager::Initialize();
	GCLAssert(mRenderer == NULL);
	mRenderer = new GLRenderer();
}
/*static */void GCLApplication::Terminate()
{
	GCLAssert(mRenderer);
	delete mRenderer;
	mRenderer = NULL;
	TextureResourceManager::Terminate();
}



void GCLApplication::Update()
{
	Input::ProcessInput();
}
void GCLApplication::Render()
{
	GCLAssert(mRenderer);
	mRenderer->Render(mRenderObjectList);
}

void GCLApplication::SetViewportCamera(const Camera &camera)
{
	mRenderer->SetCamera(camera);
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
