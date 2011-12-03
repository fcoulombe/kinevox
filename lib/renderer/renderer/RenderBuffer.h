#pragma once
#include "renderer/OpenGL.h"


namespace GCL
{
class RenderBuffer
{
public:
	RenderBuffer(size_t width, size_t height)
	: mRenderBufferId(-1)
	{
		glGenRenderbuffersEXT(1, &mRenderBufferId); glErrorCheck();
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mRenderBufferId);glErrorCheck();
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);glErrorCheck();
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0); glErrorCheck();
	}
	~RenderBuffer()
	{
		glDeleteRenderbuffers(1, &mRenderBufferId); glErrorCheck();
	}
	void Bind() { GCLAssert(IsValid()); glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferId); glErrorCheck(); }

	bool IsValid() const { return (int)mRenderBufferId!=-1; }

	void Save(const char * /*filename*/) { GCLAssert(false && "TBD"); }

	static void ResetDefault() { GCLAssert(false && "TBD"); }

private:
	friend class FrameBuffer;
	GLuint GetRenderBufferId() const  { return  mRenderBufferId; }
	GLuint mRenderBufferId;
};

}
