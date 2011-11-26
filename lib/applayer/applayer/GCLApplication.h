#pragma once

#include <vector>
#include <gcl/GCLExport.h>
#include <renderer/Renderer.h>

namespace GCL
{
class Camera;
class GLRenderer;
class GCLRenderObject;
class Input;

class GCLApplication
{
public:
	GCLEXPORT static void Initialize();
	GCLEXPORT static void Terminate();

	GCLEXPORT static void Update();
	GCLEXPORT static void Render();

	GCLEXPORT static void SetViewportCamera(const Camera &camera);


	GCLTEST_EXPORT static bool IsRegistered(const GCLRenderObject &obj);
private:
	GCLApplication();
	~GCLApplication();

	friend class GCLRenderObject;
	static void RegisterRenderObject(GCLRenderObject* newRenderObject);
	static void ReleaseRenderObject(GCLRenderObject* renderObjectToDelete);


	static GLRenderer *mRenderer;

	static RenderObjectList mRenderObjectList;
};

}
