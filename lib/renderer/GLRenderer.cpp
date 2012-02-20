
#include "renderer/GLRenderer.h"

#include <sstream>
//#if !defined(ES1) && !defined(ES2)
#include <SDL.h>
#include <SDL_video.h>
//#endif

#include <gcl/Assert.h>
#include <gcl/StringUtil.h>

#include "renderer/GLRenderUtils.h"
#include "renderer/Material.h"
#include "renderer/OpenGL.h"
#include "renderer/RenderObject.h"
#include "renderer/Shader.h"
#include "renderer/VertexBuffer.h"



using namespace GCL;


void GLRenderer::Init3DState()
{

	int sdlInitSuccessful = SDL_Init(SDL_INIT_VIDEO);
	GCLAssert(sdlInitSuccessful>= 0);

	const SDL_VideoInfo* info = NULL;
	int width = mViewPort.GetWidth();
	int height = mViewPort.GetHeight();
#ifdef ES1
	int flags = SDL_OPENGL | SDL_WINDOW_SHOWN;
#else
	int flags = SDL_OPENGL;
#endif

#if 0 //defined(OS_IPHONE)
	SDL_Window *window;
	SDL_Renderer *r;
	/* create window and renderer */
	window =
			SDL_CreateWindow(NULL, 0, 0, width, height,
					SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) {
		printf("Could not initialize Window\n");
		return 1;
	}

	r = SDL_CreateRenderer(window, -1, 0);
	if (!r) {
		printf("Could not create renderer\n");
		return 1;
	}

#else
	info = SDL_GetVideoInfo( );

	if( !info )
	{
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
	//std::cout << "GLRenderer::Init3DState> w " << width << " h " <<  height << " bpp " << bpp << " f " << flags ;
	if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 )
	{
		std::stringstream s;
		s<<"Video mode set failed: " <<	SDL_GetError( ) << std::endl;
		GCLAssertMsg(false,  s.str().c_str());
		return;
	}
#endif

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
#if !defined(ES1) && !defined(ES2)
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); glErrorCheck();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL); glErrorCheck();
#endif


	glViewport(0,0,width,height); glErrorCheck();

}

void GLRenderer::Init2DState()
{

#if !defined(ES1) && !defined(ES2)
	int sdlInitSuccessful = SDL_Init(SDL_INIT_VIDEO);
	GCLAssert(sdlInitSuccessful>= 0);

	// tell system which funciton to process when exit() call is made
	atexit(SDL_Quit);

	const SDL_VideoInfo* info = NULL;
	int width = 1024;
	int height = 768;
	info = SDL_GetVideoInfo( );

	if( !info )
	{
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
#else

	GCLAssert(false && "TBD");
#endif

}



GLRenderer::GLRenderer()
{
	mCamera=&Camera::DefaultCamera();
	Init3DState();

	mVersion = std::string((const char*)glGetString(GL_VERSION)); glErrorCheck();
	mVendor = std::string((const char*)glGetString(GL_VENDOR));glErrorCheck();
	mRenderer = std::string((const char*)glGetString(GL_RENDERER));glErrorCheck();
#if !defined(ES1)
	mShadingLanguageVersion = std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));glErrorCheck();
#endif
	char delim = ' ';
	std::string extString((const char *) glGetString(GL_EXTENSIONS));
	mExtensions = StringUtil::Explode(extString, delim); glErrorCheck();

#if ENABLE_GLEW
	GLenum err = glewInit();
	GCLAssertMsg(GLEW_OK == err, (const char *)glewGetErrorString(err));
	mGlewVersion = std::string((const char*)glewGetString(GLEW_VERSION));
#else
	mGlewVersion  = std::string("Unused");
#endif
}
GLRenderer::~GLRenderer()
{
	SDL_Quit();
}

bool GLRenderer::Update()
{

	return true;
}


void GLRenderer::PreRender()
{
	glClearColor(0.0, 0.0, 1.0, 0.0); glErrorCheck();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); glErrorCheck();

}
void GLRenderer::PostRender()
{
	SDL_GL_SwapBuffers();glErrorCheck();

}

void GLRenderer::Render(const RenderObjectList &renderObjectList)
{

	mCamera->Update();

	const Matrix44 &projection = mCamera->GetProjection();
#if ENABLE_FIX_PIPELINE
	glMatrixMode(GL_PROJECTION);
	glLoadMatrix(reinterpret_cast<const GLreal*>(&projection)); glErrorCheck();
#endif

	const Matrix44 &modelView = mCamera->GetModelView();
#if ENABLE_FIX_PIPELINE
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrix(reinterpret_cast<const GLreal*>(&modelView));
#endif

	for (size_t i=0;  i<renderObjectList.size(); ++i)
	{
#if 0
		glPushMatrix();glErrorCheck();
		glTranslatef(0.0,0.0,-10.0);

		glBegin (GL_TRIANGLE_STRIP);
		glTexCoord2f (0.0, 0.0);
		glVertex3f (-0.5, -0.5, 0.0);
		glTexCoord2f (1.0, 0.0);
		glVertex3f (0.5, -0.5, 0.0);
		glTexCoord2f (0.0, 1.0);
		glVertex3f (-0.5, 0.5, 0.0);
		glTexCoord2f (1.0, 1.0);
		glVertex3f (0.5, 0.5, 0.0);
		glEnd ();
		glPopMatrix();glErrorCheck();
#else

		const Material &tempMaterial = renderObjectList[i]->GetMaterial();
		tempMaterial.Bind();

		const Matrix44 &transform = renderObjectList[i]->GetTransform();
		SetTransform(projection, modelView, transform);

		//FC: can sort by component type
		const VertexData &data = renderObjectList[i]->GetVertexData();
		switch (data.mVertexType)
		{
		case ePOSITION:
		{
			VertexBuffer<VertexP> buffer((const VertexP *)data.mVertexData, data.mVertexCount);
			buffer.Render();
		}
		break;
		case ePOSITION|eNORMAL:
	{
		VertexBuffer<VertexPN> buffer((const VertexPN *)data.mVertexData, data.mVertexCount);
		buffer.Render();
	}
	break;
		case ePOSITION|eTEXTURE_COORD:
	{
		VertexBuffer<VertexPT> buffer((const VertexPT *)data.mVertexData, data.mVertexCount);
		buffer.Render();
	}
	break;
		case ePOSITION|eNORMAL|eTEXTURE_COORD:
	{

		VertexBuffer<VertexPNT> buffer((const VertexPNT *)data.mVertexData, data.mVertexCount);
		buffer.Render();
	}
	break;
		}
#endif
	}

	/*for (size_t i=0; i<10; ++i) {
		for (size_t j=0; j<10; ++j) {
				DrawCube(WorldPoint3(i*1.0,  0, j*1.0), 1.0);
		}
	}*/

}

void GLRenderer::Render(const SpriteList &spriteList)
{
	for (size_t i=0;  i<spriteList.size(); ++i)
	{
		glPushMatrix();glErrorCheck();
		glTranslatef(0.0,0.0,-10.0);

		glBegin (GL_TRIANGLE_STRIP);
		glTexCoord2f (0.0, 0.0);
		glVertex3f (-0.5, -0.5, 0.0);
		glTexCoord2f (1.0, 0.0);
		glVertex3f (0.5, -0.5, 0.0);
		glTexCoord2f (0.0, 1.0);
		glVertex3f (-0.5, 0.5, 0.0);
		glTexCoord2f (1.0, 1.0);
		glVertex3f (0.5, 0.5, 0.0);
		glEnd ();
		glPopMatrix();glErrorCheck();
	}
}


static bool isRenderingExtra = false;
void GLRenderer::RenderExtra(uint8_t *rgb_front, size_t width, size_t height, size_t depth)
{
#if !defined(ES1) && !defined(ES2)
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
#else
	GCLAssert(false &&" TBD");
#endif

}
void GLRenderer::Render(uint8_t *rgb_front, uint8_t *depth_front)
{
#if !defined(ES1) && !defined(ES2)

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
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_front); glErrorCheck();

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
	glTexCoord2f(0, 0); glVertex3f(640,0,0);
	glTexCoord2f(1, 0); glVertex3f(1280,0,0);
	glTexCoord2f(1, 1); glVertex3f(1280,480,0);
	glTexCoord2f(0, 1); glVertex3f(640,480,0);
	glEnd(); glErrorCheck();


	SDL_GL_SwapBuffers();glErrorCheck();
#else 
	GCLAssert(false && "TBD");
#endif

}

void GLRenderer::RenderState::SetTextureEnabled(bool isEnabled)
{
	if (isEnabled)
	{
		glEnable(GL_TEXTURE_2D); glErrorCheck();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);  glErrorCheck();
		glDisable(GL_TEXTURE_2D); glErrorCheck();
	}
}


Matrix44 GLRenderer::GetGLProjection()
{
	Matrix44f projectionMatrix;
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&projectionMatrix);
	Matrix44 projectionMatrixd;
	projectionMatrixd[0].x = projectionMatrix.m0.x;
	projectionMatrixd[0].y = projectionMatrix.m0.y;
	projectionMatrixd[0].z = projectionMatrix.m0.z;
	projectionMatrixd[0].w = projectionMatrix.m0.w;
	projectionMatrixd[1].x = projectionMatrix.m1.x;
	projectionMatrixd[1].y = projectionMatrix.m1.y;
	projectionMatrixd[1].z = projectionMatrix.m1.z;
	projectionMatrixd[1].w = projectionMatrix.m1.w;
	projectionMatrixd[2].x = projectionMatrix.m2.x;
	projectionMatrixd[2].y = projectionMatrix.m2.y;
	projectionMatrixd[2].z = projectionMatrix.m2.z;
	projectionMatrixd[2].w = projectionMatrix.m2.w;
	projectionMatrixd[3].x = projectionMatrix.m3.x;
	projectionMatrixd[3].y = projectionMatrix.m3.y;
	projectionMatrixd[3].z = projectionMatrix.m3.z;
	projectionMatrixd[3].w = projectionMatrix.m3.w;
	return projectionMatrixd;
}
Matrix44 GLRenderer::GetGLModelView()
{
	Matrix44f modelViewMatrix;
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&modelViewMatrix);
	Matrix44 modelViewMatrixd;
	modelViewMatrixd[0].x = modelViewMatrix.m0.x;
	modelViewMatrixd[0].y = modelViewMatrix.m0.y;
	modelViewMatrixd[0].z = modelViewMatrix.m0.z;
	modelViewMatrixd[0].w = modelViewMatrix.m0.w;
	modelViewMatrixd[1].x = modelViewMatrix.m1.x;
	modelViewMatrixd[1].y = modelViewMatrix.m1.y;
	modelViewMatrixd[1].z = modelViewMatrix.m1.z;
	modelViewMatrixd[1].w = modelViewMatrix.m1.w;
	modelViewMatrixd[2].x = modelViewMatrix.m2.x;
	modelViewMatrixd[2].y = modelViewMatrix.m2.y;
	modelViewMatrixd[2].z = modelViewMatrix.m2.z;
	modelViewMatrixd[2].w = modelViewMatrix.m2.w;
	modelViewMatrixd[3].x = modelViewMatrix.m3.x;
	modelViewMatrixd[3].y = modelViewMatrix.m3.y;
	modelViewMatrixd[3].z = modelViewMatrix.m3.z;
	modelViewMatrixd[3].w = modelViewMatrix.m3.w;
	return modelViewMatrixd;
}


void GLRenderer::SetTransform( const Matrix44 &projection, const Matrix44 &modelView, const Matrix44 &transform, Shader *shader)
{
	Matrix44 f = modelView*transform;

#if ENABLE_FIX_PIPELINE
	glMatrixMode(GL_PROJECTION);
	glLoadMatrix(reinterpret_cast<const GLreal*>(&projection));


	glMatrixMode(GL_MODELVIEW);
	glLoadMatrix(reinterpret_cast<const GLreal*>(&f));
	(void)shader;
#else
	if (shader)
	{
		shader->SetProjectionMatrix(projection);
		shader->SetModelViewMatrix(f);
	}
#endif
}
