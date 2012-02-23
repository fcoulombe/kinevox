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

#include <SDL.h>
#include <gcl/Assert.h>

/*#include <renderer/Camera.h>
#include <renderer/CVRenderer.h>
#include <renderer/GLRenderer.h>
#include <renderer/GLRenderUtils.h>
#include <renderer/OpenGL.h>
#include <renderer/Renderer.h>
#include <renderer/RenderObject.h>
#include <renderer/Shader.h>
#include <renderer/ShaderAttributeDefaultLocations.h>
#include <renderer/Vertex.h>
#include <renderer/VertexBuffer.h>*/

#include "CameraTest.h"
#include "FrameBufferTest.h"
#include "GeomUtilTest.h"
#include "GLRendererTest.h"
#include "MaterialTest.h"
#include "MeshTest.h"
#include "PngLoadingTest.h"
#include "RenderBufferTest.h"
#include "RenderObjectTest.h"
#include "RenderObject2DTest.h"
#include "RenderObjectWithMaterialTest.h"
#include "ResourceTest.h"
#include "ShaderTest.h"
#include "SpriteTest.h"
#include "TextureTest.h"
#include "TextureAndShaderTest.h"
#include "TgaLoadingTest.h"
#include "VertexBufferTest.h"
#include "VertexTest.h"

#include <unistd.h>

using namespace GCL;




int main(int /*argc*/, char **argv)
{
  std::cout << "[TEST]" << std::string(argv[0]) << std::endl;

  try
  {
      GLRendererTest::Test();
      CameraTest::Test();

      ShaderTest::Test();
      VertexTest::Test();
      VertexBufferTest::Test();
      TgaLoadingTest::Test();
      PngLoadingTest::Test();
      ResourceTest::Test();
      TextureTest::Test();
      TextureAndShaderTest::Test();
      MaterialTest::Test();

      SpriteTest::Test();
      RenderObject2DTest::Test();

      RenderObjectTest::Test();
      RenderObjectWithMaterialTest::Test();

      RenderBufferTest::Test();
      FrameBufferTest::Test();
      GeomUtilTest::Test();
      MeshTest::Test();

  }
  catch (GCLException & e)
  {
      std::stringstream str;
      str << "[FAILED] " << argv[0] << std::endl;
      str << e.what();
      str << std::endl;
      std::cerr << str.str();
      return -1;
  }
  catch (...)
  {
      std::cerr << "[FAILED] " << argv[0] << std::endl;
      std::cerr << "something went wrong" << std::endl;
  }
  std::cout.flush();
  std::cerr.flush();
  return 0;
}
