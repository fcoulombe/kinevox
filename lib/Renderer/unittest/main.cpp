#include <iostream>
#include <sstream>

#include <SDL.h>
#include <GCL/Assert.h>

/*#include <Renderer/Camera.h>
#include <Renderer/CVRenderer.h>
#include <Renderer/GLRenderer.h>
#include <Renderer/GLRenderUtils.h>
#include <Renderer/OpenGL.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderObject.h>
#include <Renderer/Shader.h>
#include <Renderer/ShaderAttributeDefaultLocations.h>
#include <Renderer/Vertex.h>
#include <Renderer/VertexBuffer.h>*/
#include "GLRendererTest.h"
#include "RenderObjectTest.h"
#include "RenderBufferTest.h"
#include "FrameBufferTest.h"
#include "ShaderTest.h"
#include "TextureTest.h"
#include "VertexBufferTest.h"
#include "VertexTest.h"

using namespace GCL;

int main(int argc, char **argv)
{
  try
  {
      GLRendererTest::Test();
      ShaderTest::Test();
      VertexTest::Test();
      VertexBufferTest::Test();
      TextureTest::Test()
      RenderObjectTest::Test();
      RenderBufferTest::Test();
      FrameBufferTest::Test();
  }
  catch (GCLException & e)
  {
      std::stringstream str;
      str << "FAILED: ";
      str << e.what();
      str << std::endl;
      std::cout << str.str();
      //GLRendererTest::Assert_Test(false && str.str().c_str());
      return -1;
  }

  return 0;
}
