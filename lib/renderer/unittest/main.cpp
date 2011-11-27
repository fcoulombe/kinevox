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
#include "MaterialTest.h"
#include "RenderObjectWithMaterialTest.h"
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

      RenderObjectTest::Test();
      RenderObjectWithMaterialTest::Test();

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
