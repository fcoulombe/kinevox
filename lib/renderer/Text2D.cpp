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
#include "renderer/VertexBuffer.h"


using namespace GCL;

GCL::Text2D::Text2D( const char *text ) : mScale(1.0,1.0),
	mIsVisible(true),
	mFont(FONT_PATH"FreeMono.ttf")
{
	PixelBuffer buffer;
	mFont.BlitText(buffer, text, 18, 100, 100);
	mTexture = new Texture(buffer);
}

void Text2D::Render()
{
	GCLAssert(mTexture);

	WorldPoint2 topTextureCoord, bottomTextureCoord;

	topTextureCoord.x = 0.0;
	topTextureCoord.y = 0.0;
	bottomTextureCoord.x = 1.0;
	bottomTextureCoord.y = 1.0;
	const Texture &texture = *mTexture;
	texture.Bind();

    size_t width = texture.GetWidth();
    size_t height = texture.GetHeight();
    Real widthRatio = Real(width);
    Real heightRatio = Real(height);
    Real halfWidth = (widthRatio/2.0)*mScale.x;
    Real halfHeight = (heightRatio/2.0)*mScale.y;

    VertexPT square[4];
    square[0].position = Point3<MeshReal>(MeshReal(halfWidth+mPosition.x), MeshReal(-halfHeight+mPosition.y), MeshReal(0.0));
    square[0].textureCoordinate = Point2<MeshReal>(MeshReal(bottomTextureCoord.x), MeshReal(topTextureCoord.y));

    square[1].position =Point3<MeshReal>(MeshReal(-halfWidth+mPosition.x), MeshReal(-halfHeight+mPosition.y), MeshReal(0.0));
    square[1].textureCoordinate = Point2<MeshReal>(MeshReal(topTextureCoord.x), MeshReal(topTextureCoord.y));

    square[2].position = Point3<MeshReal>(MeshReal(halfWidth+mPosition.x), MeshReal(halfHeight+mPosition.y), MeshReal(0.0));
    square[2].textureCoordinate = Point2<MeshReal>(MeshReal(bottomTextureCoord.x), MeshReal(bottomTextureCoord.y));

    square[3].position = Point3<MeshReal>(MeshReal(-halfWidth+mPosition.x), MeshReal(halfHeight+mPosition.y), MeshReal(0.0));
    square[3].textureCoordinate = Point2<MeshReal>(MeshReal(topTextureCoord.x), MeshReal(bottomTextureCoord.y));
    VertexBuffer<VertexPT> buffer(square, 4);
    buffer.PreRender();
    buffer.Render(GL_TRIANGLE_STRIP);
    buffer.PostRender();
}
