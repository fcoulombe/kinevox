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

#include <iostream>
#include <sstream>
#ifdef USE_VLD
#include <vld.h>
#endif
#include <gcl/Assert.h>

#include "AntiAliasTest.h"
#include "CameraTest.h"
#include "FontResourceTest.h"
#include "FrameBufferTest.h"
#include "GeomUtilTest.h"

#include "GLRendererTest.h"
#include "GLRendererThreadedTest.h"
#include "RLRendererTest.h"
//#include "D3DRendererTest.h"
#include "MaterialTest.h"
#include "MeshTest.h"
#include "MeshLoadingTest.h"
#include "MeshResourceTest.h"
#include "PixelBufferHALTest.h"
#include "RenderBufferTest.h"
#include "RenderObjectTest.h"
#include "RenderObjectWithMaterialTest.h"
#include "TTFFontTest.h"
#include "TextureResourceTest.h"
#include "ShaderTest.h"

#include "Text2DTest.h"
#include "TextureTest.h"
#include "TextureAndShaderTest.h"

#include "VertexBufferTest.h"
#include "VertexTest.h"
//#include <vld.h>

using namespace GCL;
int main(int argc, char **argv)
{
    std::cout << "[TEST]" << std::string(argv[0]) << std::endl;
    SUITE_INIT(argc, argv)
#if 0
    D3DRendererTest::Test();
#endif
#if USE_OPENRL
    RLRendererTest::Test();
#endif

    //     GLRendererThreadedTest::Test();
    GLRendererTest::Test();

    CameraTest::Test();

    ShaderTest::Test();

    VertexTest::Test();
    VertexBufferTest::Test();
    PixelBufferHALTest::Test();

    TextureResourceTest::Test();
    TextureTest::Test();

    TextureAndShaderTest::Test();

    RenderObjectTest::Test();
    MaterialTest::Test();

    FontResourceTest::Test();
    TTFFontTest::Test();
    Text2DTest::Test();

    RenderObjectWithMaterialTest::Test();
    GeomUtilTest::Test();

    RenderBufferTest::Test();
    FrameBufferTest::Test();

    MeshLoadingTest::Test();
    MeshResourceTest::Test();
    MeshTest::Test();
    AntiAliasTest::Test();

    SUITE_TERMINATE
    return 0;
}
