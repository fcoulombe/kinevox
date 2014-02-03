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
#include <renderer/GPUProgram.h>
#include <renderer/PixelBufferHAL.h>
#include <renderer/Shader.h>

using namespace GCL;
namespace PixelBufferHALTest
{
void Test();
void Test()
{
	TEST_START

	ShaderResourceManager::Initialize();
	{
	WinDriver winDriver("PixelBufferHALTest");
	Renderer renderer(winDriver.GetWindowsHandle());

	Shader vertexShader("DefaultVertexShader", VERTEX_SHADER);
	Shader fragmentShader("DefaultFragmentShader", FRAGMENT_SHADER);
	GPUProgram program(vertexShader, fragmentShader);
	{
	PixelBufferHAL buffer;
	buffer.IsValid();
	buffer.Bind();
	}
	{
		static const size_t BUFFER_SIZE = 64*64;
		PixelRGB buffer[BUFFER_SIZE];
		for (size_t i=0; i<BUFFER_SIZE; ++i)
		{
			buffer[i].mColor.x = uint8_t(0xffffff00^i);
			buffer[i].mColor.y =uint8_t((0xffff00ff^i)>>8);
			buffer[i].mColor.z =0;
		}
		PixelBufferHAL pb(buffer, 64, 64);
#if !defined(ES2)
		Assert_Test(pb.IsValid());
#else
		Assert_Test(!pb.IsValid()); //pbo are not supported on es2 so this can't return true
#endif
		pb.Bind();
		pb.PushData();
		pb.UnBind();
	}
	{
		const char *fullFileName = TEXTURE_PATH"mushroomtga.tga";
		std::fstream fp(fullFileName, std::fstream::binary|std::fstream::in);
		AssertMsg_Test( fp.good(), fullFileName);

		PixelBufferHAL pb;
		PixelBuffer::LoadTga(fp, pb);
		pb.Bind();
		pb.PushData();
		pb.PullData();
#if !defined(ES2)
		PixelBuffer::SaveTga("PBOTest.tga", pb.mWidth, pb.mHeight, pb.mBytesPerPixel, pb.mPixels);
#endif
	}
	}

	ShaderResourceManager::Terminate();
}
}
