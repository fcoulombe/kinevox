#include "Renderer/GLRenderer.h"

#include <SDL.h>
#include <SDL_video.h>

#include <GCL/Assert.h>
#include <Kinect/KinectDriver.h>

#include "Renderer/GLRenderUtils.h"
#include "Renderer/RenderObject.h"
#include "Renderer/VertexBuffer.h"


using namespace GCL;

void OpenGLRenderer::Init3DState()
{
  int sdlInitSuccessful = SDL_Init(SDL_INIT_VIDEO);
  GCLAssert(sdlInitSuccessful>= 0);

  const SDL_VideoInfo* info = NULL;
  int width = mViewPort.GetWidth();
  int height = mViewPort.GetHeight();
  int flags = SDL_OPENGL;
  info = SDL_GetVideoInfo( );

  if( !info ) {
      fprintf( stderr, "Video query failed: %s\n",
          SDL_GetError( ) );
      return;

  }
  int bpp = info->vfmt->BitsPerPixel;

  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
      fprintf( stderr, "Video mode set failed: %s\n",
          SDL_GetError( ) );
      return;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); glErrorCheck();
  glClearDepth(1.0); glErrorCheck();
  glDepthMask(GL_TRUE); glErrorCheck();
  glDepthFunc(GL_LESS); glErrorCheck();
  glEnable(GL_DEPTH_TEST); glErrorCheck();
  glDisable(GL_BLEND); glErrorCheck();
  glDisable(GL_ALPHA_TEST); glErrorCheck();
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glErrorCheck();
  glShadeModel(GL_FLAT); glErrorCheck();
  glEnable(GL_TEXTURE_2D); glErrorCheck();
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  glViewport(0,0,width,height); glErrorCheck();
  glMatrixMode(GL_PROJECTION); glErrorCheck();
  glLoadIdentity(); glErrorCheck();
  gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f); glErrorCheck();
  glMatrixMode(GL_MODELVIEW); glErrorCheck();
  glLoadIdentity(); glErrorCheck();


}

void OpenGLRenderer::Init2DState()
{
  int sdlInitSuccessful = SDL_Init(SDL_INIT_VIDEO);
  GCLAssert(sdlInitSuccessful>= 0);

  // tell system which funciton to process when exit() call is made
  atexit(SDL_Quit);

  const SDL_VideoInfo* info = NULL;
  int width = 1024;
  int height = 768;
  info = SDL_GetVideoInfo( );

  if( !info ) {
      fprintf( stderr, "Video query failed: %s\n", SDL_GetError( ) );
      return;

  }


  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 4 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 4 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 4 );
  SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 4 );
#ifdef __APPLE__
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      32);
#else
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      16);
#endif
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  if( SDL_SetVideoMode( width, height, info->vfmt->BitsPerPixel, SDL_OPENGL ) == 0 ) {
      fprintf( stderr, "Video mode set failed: %s\n",
          SDL_GetError( ) );
      return;

  }


  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); glErrorCheck();
  glClearDepth(1.0); glErrorCheck();
  glDepthMask(GL_FALSE); glErrorCheck();
  glDepthFunc(GL_LESS); glErrorCheck();
  glDisable(GL_DEPTH_TEST); glErrorCheck();
  glDisable(GL_BLEND); glErrorCheck();
  glDisable(GL_ALPHA_TEST); glErrorCheck();
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glErrorCheck();
  glShadeModel(GL_FLAT); glErrorCheck();
  glEnable(GL_TEXTURE_2D); glErrorCheck();

  glGenTextures(1, &gl_depth_tex); glErrorCheck();
  glBindTexture(GL_TEXTURE_2D, gl_depth_tex); glErrorCheck();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glErrorCheck();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glErrorCheck();
  glGenTextures(1, &gl_rgb_tex); glErrorCheck();
  glBindTexture(GL_TEXTURE_2D, gl_rgb_tex); glErrorCheck();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glErrorCheck();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glErrorCheck();
  glViewport(0,0,width,height); glErrorCheck();
  glMatrixMode(GL_PROJECTION); glErrorCheck();
  glLoadIdentity(); glErrorCheck();
  glOrtho (0, width, height, 0, -1.0f, 1.0f); glErrorCheck();
  glMatrixMode(GL_MODELVIEW); glErrorCheck();
  glLoadIdentity(); glErrorCheck();

}

OpenGLRenderer::OpenGLRenderer()
{
  Init3DState();

}
OpenGLRenderer::~OpenGLRenderer()
{
  /* Delete our opengl context, destroy our window, and shutdown SDL */
  SDL_Quit();
}

bool OpenGLRenderer::Update()
{

  return true;
}

void OpenGLRenderer::Render()
{
  glClearColor(0.0, 0.0, 1.0, 0.0); glErrorCheck();
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); glErrorCheck();
  glLoadIdentity(); glErrorCheck();
  mCamera.Update();

  for (size_t i=0; i<mRenderObjectList.size(); ++i) {
      const VertexData *data = mRenderObjectList[i]->GetVertexData();
      //FC: can sort by component type
      switch (data->vertexType)
      {
      case ePOSITION:
        {
          VertexBuffer<VertexP> buffer((const VertexP *)data->mVertexData, data->vertexCount);
          buffer.Render();
          break;
        }
      case ePOSITION|eNORMAL:
      {
        VertexBuffer<VertexPN> buffer((const VertexPN *)data->mVertexData, data->vertexCount);
        buffer.Render();
        break;
      }
      case ePOSITION|eTEXTURE_COORD:
      {
        VertexBuffer<VertexPT> buffer((const VertexPT *)data->mVertexData, data->vertexCount);
        buffer.Render();

        break;
      }
      case ePOSITION|eNORMAL|eTEXTURE_COORD:
      {
        VertexBuffer<VertexPNT> buffer((const VertexPNT *)data->mVertexData, data->vertexCount);
        buffer.Render();

        break;
      }
      }
  }


  /*for (size_t i=0; i<10; ++i) {
		for (size_t j=0; j<10; ++j) {
				DrawCube(WorldPoint3(i*1.0,  0, j*1.0), 1.0);
		}
	}*/

  /*glBegin(GL_QUADS);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd(); glErrorCheck();
   */
  SDL_GL_SwapBuffers();glErrorCheck();

}


static bool isRenderingExtra = false;
void OpenGLRenderer::RenderExtra(uint8_t *rgb_front, size_t width, size_t height, size_t depth)
{
  glBindTexture(GL_TEXTURE_2D, gl_depth_tex);glErrorCheck();
  if (depth == 1) {
      glTexImage2D(GL_TEXTURE_2D, 0, depth, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, rgb_front); glErrorCheck();
  }
  else if (depth == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, depth, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_front); glErrorCheck();
  }
  glBegin(GL_TRIANGLE_FAN);
  glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
  glTexCoord2f(0, 0); glVertex3f(0,0,0);
  glTexCoord2f(1, 0); glVertex3f(640,0,0);
  glTexCoord2f(1, 1); glVertex3f(640,480,0);
  glTexCoord2f(0, 1); glVertex3f(0,480,0);
  glEnd(); glErrorCheck();
  isRenderingExtra = true;

}
void OpenGLRenderer::Render(uint8_t *rgb_front, uint8_t *depth_front)
{

  if (!isRenderingExtra) {
      glBindTexture(GL_TEXTURE_2D, gl_depth_tex); glErrorCheck();
      glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, depth_front); glErrorCheck();

      glBegin(GL_TRIANGLE_FAN);
      glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
      glTexCoord2f(0, 0); glVertex3f(0,0,0);
      glTexCoord2f(1, 0); glVertex3f(640,0,0);
      glTexCoord2f(1, 1); glVertex3f(640,480,0);
      glTexCoord2f(0, 1); glVertex3f(0,480,0);
      glEnd(); glErrorCheck();
  } else {
      isRenderingExtra = false;
  }


  glBindTexture(GL_TEXTURE_2D, gl_rgb_tex); glErrorCheck();
  if (GetKinectVideoFormat() == FREENECT_VIDEO_RGB || GetKinectVideoFormat() == FREENECT_VIDEO_YUV_RGB)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_front); glErrorCheck();
    }
  else
    {
      glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, rgb_front+640*4); glErrorCheck();
    }

  glBegin(GL_TRIANGLE_FAN);
  glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
  glTexCoord2f(0, 0); glVertex3f(640,0,0);
  glTexCoord2f(1, 0); glVertex3f(1280,0,0);
  glTexCoord2f(1, 1); glVertex3f(1280,480,0);
  glTexCoord2f(0, 1); glVertex3f(640,480,0);
  glEnd(); glErrorCheck();


  SDL_GL_SwapBuffers();glErrorCheck();

}

