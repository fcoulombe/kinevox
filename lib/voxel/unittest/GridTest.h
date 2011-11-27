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
#include <renderer/GLRenderer.h>
#include <renderer/Shader.h>
#include <renderer/TextureResourceManager.h>

#include "Voxel/Grid.h"

using namespace GCL;
namespace GridTest
{
void Test()
{
	TEST_START
	TextureResourceManager::Initialize();

	GLRenderer *renderer = new GLRenderer();


	Shader shader;
	shader.Bind();

	Grid::Row row(10,10,10);
	Grid::Slice slice(10,10,10);

	Grid grid(10,10,10);
	grid.CreateVertexData();
	RenderObjectList renderList;
	renderList.push_back(&grid);

	renderer->Render(renderList);

	delete renderer;
	TextureResourceManager::Terminate();

}
}
