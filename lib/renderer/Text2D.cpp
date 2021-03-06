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

#include "renderer/Text2D.h"

using namespace GCL;

GCL::Text2D::Text2D( const char *text ) 
	: mTransform(true),
	  mFont(FONT_PATH"FreeMono.ttf"),
	  mIsVisible(true),
	  mMaterial("Default")

{

	mFont.BlitText(buffer, text, 18, 100, 100);
	mTexture = new Texture(buffer);

	MeshReal halfWidth = MeshReal(buffer.mWidth/2.0);
	MeshReal halfHeight = MeshReal(buffer.mHeight/2.0);
	square[0] = VertexP(Point3<MeshReal>(-halfWidth, -halfHeight, 0.0));
	square[1] = VertexP(Point3<MeshReal>(halfWidth, -halfHeight, 0.0));
	square[2] = VertexP(Point3<MeshReal>(-halfWidth, halfHeight, 0.0));
	square[3] = VertexP(Point3<MeshReal>(-halfWidth, halfHeight, 0.0));
	square[4] = VertexP(Point3<MeshReal>(halfWidth, -halfHeight, 0.0));
	square[5] = VertexP(Point3<MeshReal>(halfWidth, halfHeight, 0.0));
	mObj = new RenderObject(mMaterial, square, 6);
}

void GCL::Text2D::Render(const Matrix44 &projection)
{
	const RenderObject *tempRenderObject = mObj;
	const Material &tempMaterial = tempRenderObject->GetMaterial();
	tempMaterial.Bind();
	const Matrix44 &transform = mTransform;
	GPUProgram *tempProgram = tempMaterial.GetShader();
	tempProgram->SetProjectionMatrix(projection);
	tempProgram->SetModelViewMatrix(transform);
	tempRenderObject->GetVBO().Render();
}


