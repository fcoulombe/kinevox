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
#include <renderer/Shader.h>
#include <renderer/VertexBuffer.h>

using namespace GCL;
namespace VertexBufferTest
{
void Test();
void Test()
{
	TEST_START
		ShaderResourceManager::Initialize();
	{
	WinDriver winDriver("VertexBufferTest");
	Renderer renderer(winDriver.GetWindowsHandle());
	Shader vertexShader("DefaultVertexShader", VERTEX_SHADER);
	Shader fragmentShader("DefaultFragmentShader", FRAGMENT_SHADER);
	GPUProgram program(vertexShader, fragmentShader);
	program.Bind();
	AttribLocations loc;
	loc.position = program.GetAttributeLocation("InPosition");
	loc.normal = program.GetAttributeLocation("InNormal");
	loc.texCoord = program.GetAttributeLocation("InTexCoord");
	
	VertexPNT square[4] = { {Point3<MeshReal>(-0.5, -0.5, 0.0), Point3<MeshReal>(0.0, 0.0, 1.0), Point2<MeshReal>(0.0, 0.0) } ,
			{Point3<MeshReal>(0.5, -0.5, 0.0), Point3<MeshReal>(0.0, 0.0, 1.0), Point2<MeshReal>(0.0, 0.0) } ,
			{Point3<MeshReal>(0.5, 0.5, 0.0), Point3<MeshReal>(0.0, 0.0, 1.0), Point2<MeshReal>(0.0, 0.0) } ,
			{Point3<MeshReal>(-0.5, 0.5, 0.0), Point3<MeshReal>(0.0, 0.0, 1.0), Point2<MeshReal>(0.0, 0.0) } };
	VertexBuffer vb(square, 4, loc);
	Assert_Test(vb.IsValid());


	VertexPT square2[4] = { {Point3<MeshReal>(-0.5, -0.5, 0.0), Point2<MeshReal>(0.0, 0.0)} ,
			{Point3<MeshReal>(0.5, -0.5, 0.0), Point2<MeshReal>(0.0, 0.0) } ,
			{Point3<MeshReal>(0.5, 0.5, 0.0), Point2<MeshReal>(0.0, 0.0)} ,
			{Point3<MeshReal>(-0.5, 0.5, 0.0), Point2<MeshReal>(0.0, 0.0)} };
	VertexBuffer vb2(square2, 4, loc);
	Assert_Test(vb2.IsValid());
	}
	ShaderResourceManager::Terminate();
}
}
