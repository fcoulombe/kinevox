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


#include "renderer/GL/GLRenderer.h"

#include <sstream>

#include <3rdparty/OpenGL.h>
#include <gcl/Assert.h>
#include <gcl/StringUtil.h>

#include "renderer/GL/GLRenderUtils.h"
#include "renderer/Material.h"
#include "renderer/RenderObject.h"
#include "renderer/RenderObject2D.h"
#include "renderer/Text2D.h"
#include "renderer/Shader.h"
#include "renderer/VertexBuffer.h"


using namespace GCL;

void GLRenderer::Init3DState()
{
	glErrorCheck();
    glViewport(0,0,(GLsizei)mViewPort.GetWidth(),(GLsizei)mViewPort.GetHeight()); 
	glErrorCheck();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); glErrorCheck();
	glClearDepth(1.0); glErrorCheck();
	glDepthMask(GL_TRUE); glErrorCheck();
	glDepthFunc(GL_LESS); glErrorCheck();
	glEnable(GL_DEPTH_TEST); glErrorCheck();
	glDisable(GL_BLEND); glErrorCheck();
#if ENABLE_FIX_PIPELINE
    glShadeModel(GL_FLAT); glErrorCheck();
	glDisable(GL_ALPHA_TEST); glErrorCheck();
	glEnable(GL_TEXTURE_2D); glErrorCheck();
#endif
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glErrorCheck();
	

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); glErrorCheck();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL); glErrorCheck();
}

GLRenderer::GLRenderer(size_t windowsHandle)
{
	// remember the window handle (HWND)
	mhWnd = (HWND)windowsHandle;

	// get the device context (DC)
	mhDC = GetDC( mhWnd );

	// set the pixel format for the DC
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int format = ChoosePixelFormat( mhDC, &pfd );
	SetPixelFormat( mhDC, format, &pfd );

	// create the render context (RC)
	mhRC = wglCreateContext( mhDC );
	// make it the current render context
	wglMakeCurrent( mhDC, mhRC );
#if ENABLE_GLEW
	glewExperimental=TRUE;
	GLenum err = glewInit();
	GCLAssertMsg(GLEW_OK == err, (const char *)glewGetErrorString(err));
	mGlewVersion = std::string((const char*)glewGetString(GLEW_VERSION));
	glGetError(); //glew generates an error that can be ignored. on 3.x+
	glErrorCheck();
#else
	mGlewVersion  = std::string("Unused");
#endif


	Init3DState();
	mVersion = std::string((const char*)glGetString(GL_VERSION)); glErrorCheck();
	mVendor = std::string((const char*)glGetString(GL_VENDOR));glErrorCheck();
	mRenderer = std::string((const char*)glGetString(GL_RENDERER));glErrorCheck();
#if !defined(ES1)
	const char *ver = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);glErrorCheck();
	mShadingLanguageVersion = std::string(ver);
#endif
	char delim = ' ';
#if 0
	
	const char *ex = (const char *) glGetString(GL_EXTENSIONS);
	std::string extString(ex);
	mExtensions = StringUtil::Explode(extString, delim); glErrorCheck();
#else
	GLint n, i;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);glErrorCheck();
	std::string extString;
	for (i = 0; i < n; i++)
	{
		const char *ex = (const char*)glGetStringi(GL_EXTENSIONS, i); glErrorCheck();
		extString = extString +ex;
		extString = extString + " ";
	}
	mExtensions = StringUtil::Explode(extString, delim); 
#endif


}
GLRenderer::~GLRenderer()
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( mhRC );
	ReleaseDC( mhWnd, mhDC );
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
void GLRenderer::Render(const RenderObjectList &renderObjectList)
{
	const Matrix44 &projection = mProjection;
    const Matrix44 &modelView = mModelView;

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
#if 0
		SetTransform(projection, modelView, transform, tempMaterial.GetShader());
#else
		(void)projection;
		(void)modelView;
		(void)transform;
		GCLAssert(false && "we need to port this");
#endif
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

	/*for (size_t i=0; i<10; ++i) {
		for (size_t j=0; j<10; ++j) {
				DrawCube(WorldPoint3(i*1.0,  0, j*1.0), 1.0);
		}
	}*/

}

void GLRenderer::Render(const RenderObject2DList &renderObjectList)
{
    Matrix44 ortho;
    ortho.SetOrtho(0.0, (Real)mViewPort.GetHeight(), (Real)mViewPort.GetWidth(), 0.0, -1.0, 1.0);
#if ENABLE_FIX_PIPELINE
    SetTransform(ortho, Matrix44::IDENTITY, Matrix44::IDENTITY);
#else
    Shader shader;
    shader.Bind();
    SetTransform(ortho, Matrix44::IDENTITY, Matrix44::IDENTITY, &shader);
#endif

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

void GLRenderer::Render(const Text2DList &renderObjectList)
{
    Matrix44 ortho;
    ortho.SetOrtho(0.0, (Real)mViewPort.GetHeight(), (Real)mViewPort.GetWidth(), 0.0, -1.0, 1.0);
#if ENABLE_FIX_PIPELINE
    SetTransform(ortho, Matrix44::IDENTITY, Matrix44::IDENTITY);
#else
    Shader shader;
    shader.Bind();
    SetTransform(ortho, Matrix44::IDENTITY, Matrix44::IDENTITY, &shader);
#endif

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
    Matrix44 projectionMatrixd;
        Matrix44f projectionMatrix;
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&projectionMatrix);

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
    Matrix44 modelViewMatrixd;
	Matrix44f modelViewMatrix;
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&modelViewMatrix);
	
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


void GLRenderer::SetTransform( const Matrix44 &projection, const Matrix44 &modelView, const Matrix44 &transform, GLGPUProgram *shader)
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

void GLRenderer::SwapBuffer()
{
	SwapBuffers( mhDC );
}
