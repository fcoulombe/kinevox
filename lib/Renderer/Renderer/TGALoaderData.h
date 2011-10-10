#pragma once

#include "Renderer/OpenGL.h"
namespace GCL
{

  struct TGAHeader
  {
    GLubyte Header[12];
  };

  struct TGA
  {
    GLubyte header[6];
    GLuint bytesPerPixel;
    GLuint imageSize;
    GLuint type;
    GLuint height;
    GLuint width;
    GLuint bpp;
  };

  // Uncompressed TGA Header
  const GLubyte TGAUncompressedHeader[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
  // Compressed TGA Header
  const GLubyte TGACompressedHeader[12] = {0,0,10,0,0,0,0,0,0,0,0,0};



}
