/*
 * Copyright (C) 2014 by Francois Coulombe
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

#include "applayer/RenderTarget.h"
#include <gcl/Assert.h>
#include <gcl/Log.h>
#include <gcl/Matrix44.h>
#include <gcl/Point3.h>
#include <renderer/FrameBuffer.h>
#include <renderer/Material.h>
#include <renderer/RenderBuffer.h>
#include <renderer/RenderObject.h>
#include <renderer/Texture.h>
#include <renderer/Vertex.h>

using namespace GCL;

RenderTarget::RenderTarget(size_t width, size_t height)
{
	KLogD("Create Render Target");
	mRenderBuffer = new RenderBuffer(width, height);
	mRenderBuffer->Bind();
	KLogD("Create Render Target texture");
	mTexture = new Texture(width, height, 3);
	mTexture->Bind();
	KLogD("Create Render Target Frame buffer");
	mFrameBuffer = new FrameBuffer(*mTexture, *mRenderBuffer);
	mFrameBuffer->Bind();
	KLogD("Create Render Target Material");
	mMaterial = new Material();
	mMaterial->SetTexture(mTexture);
	static const VertexPT geom[6] = {
	        VertexPT(Point3<MeshReal>(-0.5, 0.5, 0.0), Point2<MeshReal>(0.0, 0.0)), //left top
	        VertexPT(Point3<MeshReal>(0.5, -0.5, 0.0), Point2<MeshReal>(1.0, 1.0)), //right bot
	        VertexPT(Point3<MeshReal>(-0.5, -0.5, 0.0), Point2<MeshReal>(0.0, 1.0)), // left bot
	        VertexPT(Point3<MeshReal>(0.5, 0.5, 0.0), 	Point2<MeshReal>(1.0, 0.0)), //right top
	        VertexPT(Point3<MeshReal>(0.5, -0.5, 0.0), Point2<MeshReal>(1.0, 1.0)), //right bot
	        VertexPT(Point3<MeshReal>(-0.5, 0.5, 0.0), Point2<MeshReal>(0.0, 0.0)), // left top
	};
	KLogD("Create Render Target Render object");
	mBackground = new RenderObject(*mMaterial, geom, 6);
}

RenderTarget::~RenderTarget()
{
	delete mBackground;
	delete mMaterial;
	delete mFrameBuffer;
	delete mRenderBuffer;
}

void RenderTarget::Bind()
{
	mRenderBuffer->Bind();
	mTexture->Bind();
	mFrameBuffer->Bind();
}

void RenderTarget::Render(const Matrix44 &ortho)
{
	const Material &tempMaterial = mBackground->GetMaterial();
	tempMaterial.Bind();
	GPUProgram *tempProgram = tempMaterial.GetShader();
	tempProgram->SetProjectionMatrix(ortho);
	tempProgram->SetModelViewMatrix(Matrix44::IDENTITY);
	mBackground->GetVBO().Render();
}
