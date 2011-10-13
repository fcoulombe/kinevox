#pragma once

#include <vector>
#include <GCL/GCLExport.h>

namespace GCL
{
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

private:
	GCLApplication();
	~GCLApplication();

	friend class GCLRenderObject;
	static void RegisterRenderObject(GCLRenderObject* newRenderObject);
	static void ReleaseRenderObject(GCLRenderObject* renderObjectToDelete);


	static GLRenderer *mRenderer;

	typedef std::vector<GCLRenderObject*> RenderObjectList;
	static RenderObjectList mRenderObjectList;
};

}
