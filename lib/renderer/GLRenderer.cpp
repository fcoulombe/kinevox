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


#include "renderer/GLRenderer.h"

#include <sstream>

#include <3rdparty/OpenGL.h>
#include <gcl/Assert.h>
#include <gcl/StringUtil.h>

#include "renderer/GLRenderUtils.h"
#include "renderer/Material.h"
#include "renderer/RenderObject.h"
#include "renderer/RenderObject2D.h"
#include "renderer/Text2D.h"
#include "renderer/Shader.h"
#include "renderer/VertexBuffer.h"


using namespace GCL;

void GLRenderer::Init3DState()
{
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
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); glErrorCheck();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL); glErrorCheck();
#endif
}

GLRenderer::GLRenderer()
{
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
#else
	mGlewVersion  = std::string("Unused");
#endif
}
GLRenderer::~GLRenderer()
{
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
	mCamera->Update();

	const Matrix44 &projection = mCamera->GetProjection();
    const Matrix44 &modelView = mCamera->GetModelView();
#ifdef ENABLE_FIX_PIPELINE
	glMatrixMode(GL_PROJECTION);
	glLoadMatrix(reinterpret_cast<const GLreal*>(&projection)); glErrorCheck();

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

	/*for (size_t i=0; i<10; ++i) {
		for (size_t j=0; j<10; ++j) {
				DrawCube(WorldPoint3(i*1.0,  0, j*1.0), 1.0);
		}
	}*/

}

void GLRenderer::Render(const RenderObject2DList &renderObjectList, size_t viewportWidth, size_t viewportHeight)
{
#ifdef ENABLE_FIX_PIPELINE
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0, viewportHeight, viewportWidth, 0, -1.0f, 1.0f); glErrorCheck();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
		glPushMatrix();glErrorCheck();
		renderObjectList[i]->Render();
		glPopMatrix();glErrorCheck();
#endif
	}
}

void GLRenderer::Render(const Text2DList &renderObjectList, size_t viewportWidth, size_t viewportHeight)
{
#ifdef ENABLE_FIX_PIPELINE
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0, viewportHeight, viewportWidth, 0, -1.0f, 1.0f); glErrorCheck();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
		glPushMatrix();glErrorCheck();
		renderObjectList[i]->Render();
		glPopMatrix();glErrorCheck();
#endif
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

#ifdef ENABLE_FIX_PIPELINE
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
