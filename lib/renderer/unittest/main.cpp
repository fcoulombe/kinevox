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

#include "CameraTest.h"
#include "GLRendererTest.h"
#include "PngLoadingTest.h"
#include "RenderObjectTest.h"
#include "RenderBufferTest.h"
#include "FrameBufferTest.h"
#include "ResourceTest.h"
#include "ShaderTest.h"
#include "TextureTest.h"
#include "TextureAndShaderTest.h"
#include "TgaLoadingTest.h"
#include "VertexBufferTest.h"
#include "VertexTest.h"

#include <unistd.h>

using namespace GCL;



static const std::string Dirname(const std::string  &dir)
{
  const char DIR_SEPARATOR = '/';
  register int i;
  const std::string &loc = dir;
  size_t size = loc.length();

  for (i = (size - 1); (i >= 0) && (loc[i] != DIR_SEPARATOR); --i)
    ;

  return loc.substr(0, i);
}

int main(int /*argc*/, char **argv)
{
  std::cout << "App: " << Dirname(std::string(argv[0])) << std::endl;

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
      RenderObjectTest::Test();
      RenderBufferTest::Test();
      FrameBufferTest::Test();
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
  return 0;
}
