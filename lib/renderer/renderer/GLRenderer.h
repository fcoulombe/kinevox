#pragma once

#include <cstdlib>
#include <stdint.h>
#include <algorithm>
#include <string>

#include "renderer/Camera.h"
#include "renderer/Renderer.h"
#include "renderer/ViewPort.h"


namespace GCL
{

typedef std::vector<std::string> GLExtensionList;
class GLRenderer : public Renderer
{
public:
	GLRenderer();
	~GLRenderer();
	bool Update();
	virtual void Render(const RenderObjectList &renderObjectList);
	void Render(uint8_t *rgb_front, uint8_t *depth_front);
	void RenderExtra(uint8_t *rgb_front, size_t width, size_t height, size_t depth);


	void SetCamera(Camera &camera) { mCamera = &camera; }

	const ViewPort &GetViewPort() const { return mViewPort; }

	uint32_t gl_depth_tex;
	uint32_t gl_rgb_tex;

	struct RenderState
	{
		void SetTextureEnabled(bool isEnabled);
	};
	RenderState mCurrentRenderState;


	const std::string &GetVendor() const { return mVendor; }
	const std::string &GetVersion() const { return mVersion; }
	const std::string &GetRenderer() const { return mRenderer; }
	const std::string &GetShadingLanguageVersion() const { return mShadingLanguageVersion; }
	const std::string &GetGlewVersion() const { return mGlewVersion; }
	const GLExtensionList &GetExtensions() const { return mExtensions; }

	bool IsExtensionSupported(const std::string &ext) const
	{
		GLExtensionList::const_iterator b = mExtensions.begin();
		GLExtensionList::const_iterator e = mExtensions.end();
		GLExtensionList::const_iterator r =  std::find(b, e, ext);

		bool res = r != mExtensions.end();
		return res;
	}

	static void SetTransform( const Matrix44 &projection, const Matrix44 &modelView, const Matrix44 &transform);

	static Matrix44 GetGLProjection();
	static Matrix44 GetGLModelView();

private:
	void Init3DState();
	void Init2DState();

	Camera *mCamera;
	ViewPort mViewPort;

	std::string mVendor, mVersion,mRenderer, mShadingLanguageVersion, mGlewVersion;

	GLExtensionList mExtensions;




};
}
