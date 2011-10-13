#include "AppLayer/GCLApplication.h"


#include <AppLayer/GCLRenderObject.h>
#include <GCL/Assert.h>
#include <Input/Input.h>
#include <Renderer/GLRenderer.h>

using namespace GCL;

GLRenderer *GCLApplication::mRenderer = NULL;
GCLApplication::RenderObjectList GCLApplication::mRenderObjectList;


/*static */void GCLApplication::Initialize()
{
	GCLAssert(mRenderer == NULL);
	mRenderer = new GLRenderer();
}
/*static */void GCLApplication::Terminate()
{
	GCLAssert(mRenderer);
	delete mRenderer;
	mRenderer = NULL;
}



void GCLApplication::Update()
{
	ProcessInput();
}
void GCLApplication::Render()
{
	GCLAssert(mRenderer);
	mRenderer->Render();
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
