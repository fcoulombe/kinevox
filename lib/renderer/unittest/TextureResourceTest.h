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
#pragma once

#include <gcl/UnitTest.h>
#include <renderer/TextureResourceManager.h>

using namespace GCL;

namespace TextureResourceTest
{
void Test();
void Test()
{
	TEST_START

	TextureResourceManager::Initialize();

	TextureResourceManager &textureResourceManager = TextureResourceManager::Instance();
	const Resource *tgaResource;
	tgaResource = textureResourceManager.LoadResource(TEXTURE_PATH"mushroomtga.tga");
	Assert_Test(tgaResource);

	//test resource sharing
	const Resource *tgaResource2;
	tgaResource2 = textureResourceManager.LoadResource(TEXTURE_PATH"mushroomtga.tga");
	Assert_Test(tgaResource == tgaResource2);
#ifndef OS_IPHONE
	const Resource *pngResource;
	pngResource = textureResourceManager.LoadResource(TEXTURE_PATH"mushroompng.png");
	Assert_Test(pngResource);
#endif

	textureResourceManager.ReleaseResource(tgaResource);
	textureResourceManager.ReleaseResource(tgaResource2);
	textureResourceManager.ReleaseResource(pngResource);
	TextureResourceManager::Terminate();
}
}
