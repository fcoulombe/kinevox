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


#include "renderer/GLES/GLESRenderer.h"

#include <sstream>

#include <gcl/Assert.h>
#include <gcl/StringUtil.h>

#include "renderer/GLES/GLESRenderUtils.h"
#include "renderer/Material.h"
#include "renderer/RenderObject.h"
#include "renderer/RenderObject2D.h"
#include "renderer/Text2D.h"
#include "renderer/Shader.h"
#include "renderer/VertexBuffer.h"


using namespace GCL;

void GLESRenderer::Init3DState()
{

    glViewport(0,0,(GLsizei)mViewPort.GetWidth(),(GLsizei)mViewPort.GetHeight()); glErrorCheck();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); glErrorCheck();
	glClearDepth(1.0); glErrorCheck();
	glDepthMask(GL_TRUE); glErrorCheck();
	glDepthFunc(GL_LESS); glErrorCheck();
	glEnable(GL_DEPTH_TEST); glErrorCheck();
	glDisable(GL_BLEND); glErrorCheck();
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glErrorCheck();
	
	glEnable(GL_TEXTURE_2D); glErrorCheck();
}

GLESRenderer::GLESRenderer(size_t windowsHandle)
{
    HDC hDC = GetDC((HWND)windowsHandle);
    GCLAssertMsg(hDC, "Failed to create the device context");
    eglDisplay = eglGetDisplay(hDC);
    if(eglDisplay == EGL_NO_DISPLAY)
        eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);

    EGLint iMajorVersion, iMinorVersion;
    EGLBoolean ret = eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion);
    GCLAssertMsg(ret, "eglInitialize() failed.");
    ret = eglBindAPI(EGL_OPENGL_ES_API); 

#if defined(ES1)
    EGLint pi32ConfigAttribs[128];
    int i = 0;
    pi32ConfigAttribs[i++] = EGL_RED_SIZE;
    pi32ConfigAttribs[i++] = 5;
    pi32ConfigAttribs[i++] = EGL_GREEN_SIZE;
    pi32ConfigAttribs[i++] = 6;
    pi32ConfigAttribs[i++] = EGL_BLUE_SIZE;
    pi32ConfigAttribs[i++] = 5;
    pi32ConfigAttribs[i++] = EGL_ALPHA_SIZE;
    pi32ConfigAttribs[i++] = 0;
    pi32ConfigAttribs[i++] = EGL_SURFACE_TYPE;
    pi32ConfigAttribs[i++] = EGL_WINDOW_BIT;
    pi32ConfigAttribs[i++] = EGL_NONE;
#elif defined(ES2)
    const EGLint pi32ConfigAttribs[] =
    {
        EGL_LEVEL,				0,
        EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
        EGL_NATIVE_RENDERABLE,	EGL_FALSE,
        EGL_DEPTH_SIZE,			EGL_DONT_CARE,
        EGL_NONE
    };
#elif defined(ES3)
#error "TBD"
#endif
    EGLint iConfigs;
    ret = eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1);
    GCLAssertMsg(ret, "eglChooseConfig() failed.");

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);
    if (eglSurface == EGL_NO_SURFACE)
    {    
        eglGetError(); // Clear error
        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}


#if defined(ES1)
    EGLint *ai32ContextAttribs=NULL;
#elif defined(ES2)
	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
#else
#error "TBD"
#endif
    eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);
	glErrorCheck();

	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	glErrorCheck();

    mViewPort.Set(0,0,Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH"), Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT"));

	mCamera=&Camera::DefaultCamera();
	Init3DState();

	mVersion = std::string((const char*)glGetString(GL_VERSION)); glErrorCheck();
	mVendor = std::string((const char*)glGetString(GL_VENDOR));glErrorCheck();
	mRenderer = std::string((const char*)glGetString(GL_RENDERER));glErrorCheck();
#if !defined(ES1) && !defined(OS_WIN32)
	const char *ver = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);glErrorCheck();
	mShadingLanguageVersion = std::string(ver);
#endif
	char delim = ' ';
	std::string extString((const char *) glGetString(GL_EXTENSIONS));
	mExtensions = StringUtil::Explode(extString, delim); glErrorCheck();

#if ENABLE_GLEW
	GLenum err = glewInit();
	GCLAssertMsg(GLEW_OK == err, (const char *)glewGetErrorString(err));
	mGlewVersion = std::string((const char*)glewGetString(GLEW_VERSION));
#elif ENABLE_EGLEW
    mGlewVersion  = std::string("eglew");
    eglewInit();
#else
	mGlewVersion  = std::string("Unused");
#endif
}
GLESRenderer::~GLESRenderer()
{
}

bool GLESRenderer::Update()
{
	return true;
}


void GLESRenderer::PreRender()
{
	glClearColor(0.0, 0.0, 1.0, 0.0); glErrorCheck();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); glErrorCheck();
}
void GLESRenderer::PostRender()
{
}

template<typename VertexType>
void DrawNormals(const VertexData &data)
{
    std::vector<Point3<MeshReal> > normalLines;
    const VertexType *vertexData = (const VertexType *)(data.mVertexData); 
    for (size_t i=0; i<data.mVertexCount; ++i)
    {
        const VertexType &vertex = vertexData[i];
        normalLines.push_back(vertex.position);
        normalLines.push_back(vertex.position + (vertex.normal*0.5));
    }
    const VertexP *pos = (const VertexP *)(normalLines.data());
    VertexData lineData(pos, normalLines.size(), VertexP::GetComponentType());
    VertexBuffer<VertexP> buffer((const VertexP *)lineData.mVertexData, lineData.mVertexCount);
    buffer.Render(GL_LINES);

}
void GLESRenderer::Render(const RenderObjectList &renderObjectList)
{
	mCamera->Update();

	const Matrix44 &projection = mCamera->GetProjection();
    const Matrix44 &modelView = mCamera->GetModelView();

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
        const RenderObject *tempRenderObject = renderObjectList[i];
		const Material &tempMaterial = tempRenderObject->GetMaterial();
		tempMaterial.Bind();

		const Matrix44 &transform = tempRenderObject->GetTransform();
		SetTransform(projection, modelView, transform, tempMaterial.GetShader());

		//FC: can sort by component type
		const VertexDataList &dataList = tempRenderObject->GetVertexData();
        for (size_t j=0; j<dataList.size(); ++j)
        {
            const VertexData &data = dataList[j]; 
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
                    if (tempRenderObject->IsDrawingNormals())
                        DrawNormals<VertexPN>(data);
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
                    if (tempRenderObject->IsDrawingNormals())
                        DrawNormals<VertexPNT>(data);
                }
                break;
            }
        }
#endif
	}
}

void GLESRenderer::Render(const RenderObject2DList &renderObjectList)
{
    Matrix44 ortho;
    ortho.SetOrtho(0.0, (Real)mViewPort.GetHeight(), (Real)mViewPort.GetWidth(), 0.0, -1.0, 1.0);

    Shader shader;
    shader.Bind();
    SetTransform(proj, Matrix44::IDENTITY, Matrix44::IDENTITY, &shader);


	for (size_t i=0;  i<renderObjectList.size(); ++i)
	{
#if 0
		glPushMatrix();glErrorCheck();
		glTranslatef(0.0,0.0,0.0);

		glBegin (GL_TRIANGLE_STRIP);
		glTexCoord2f (0.0, 0.0);
		glVertex3f (-5.0, -5.0, 0.0);
		glTexCoord2f (1.0, 0.0);
		glVertex3f (5.0, -5.0, 0.0);
		glTexCoord2f (0.0, 1.0);
		glVertex3f (-5.0, 5.0, 0.0);
		glTexCoord2f (1.0, 1.0);
		glVertex3f (5.0, 5.0, 0.0);
		glEnd ();
		glPopMatrix();glErrorCheck();
#else
		//glPushMatrix();glErrorCheck();
		renderObjectList[i]->Render();
		//glPopMatrix();glErrorCheck();
#endif
	}
}

void GLESRenderer::Render(const Text2DList &renderObjectList)
{
    Matrix44 ortho;
    ortho.SetOrtho(0.0, (Real)mViewPort.GetHeight(), (Real)mViewPort.GetWidth(), 0.0, -1.0, 1.0);

    Shader shader;
    shader.Bind();
    SetTransform(proj, Matrix44::IDENTITY, Matrix44::IDENTITY, &shader);


	for (size_t i=0;  i<renderObjectList.size(); ++i)
	{
#if 0
		glPushMatrix();glErrorCheck();
		glTranslatef(0.0,0.0,0.0);

		glBegin (GL_TRIANGLE_STRIP);
		glTexCoord2f (0.0, 0.0);
		glVertex3f (-5.0, -5.0, 0.0);
		glTexCoord2f (1.0, 0.0);
		glVertex3f (5.0, -5.0, 0.0);
		glTexCoord2f (0.0, 1.0);
		glVertex3f (-5.0, 5.0, 0.0);
		glTexCoord2f (1.0, 1.0);
		glVertex3f (5.0, 5.0, 0.0);
		glEnd ();
		glPopMatrix();glErrorCheck();
#else
		//glPushMatrix();glErrorCheck();
		renderObjectList[i]->Render();
		//glPopMatrix();glErrorCheck();
#endif
	}
}

void GLESRenderer::RenderState::SetTextureEnabled(bool isEnabled)
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


Matrix44 GLESRenderer::GetGLProjection()
{
    Matrix44 projectionMatrixd;
    GCLAssert(false && "UNSUPPORTED");
	return projectionMatrixd;
}
Matrix44 GLESRenderer::GetGLModelView()
{
    Matrix44 modelViewMatrixd;
    GCLAssert(false && "UNSUPPORTED");
	return modelViewMatrixd;
}


void GLESRenderer::SetTransform( const Matrix44 &projection, const Matrix44 &modelView, const Matrix44 &transform, Shader *shader)
{
	Matrix44 f = modelView*transform;

	if (shader)
	{
		shader->SetProjectionMatrix(projection);
		shader->SetModelViewMatrix(f);
	}
}
