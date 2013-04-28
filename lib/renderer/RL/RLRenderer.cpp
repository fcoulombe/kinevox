/*
 * Copyright (C) 2013 by Francois Coulombe
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

#if USE_OPENRL
#include "renderer/RLRenderer.h"

#include <sstream>


#include <gcl/Assert.h>
#include <gcl/Config.h>
#include <gcl/StringUtil.h>

#include "renderer/GLRenderUtils.h"
#include "renderer/Material.h"
#include "renderer/RenderObject.h"
#include "renderer/RenderObject2D.h"
#include "renderer/Text2D.h"
#include "renderer/Shader.h"
#include "renderer/VertexBuffer.h"


using namespace GCL;

void RLRenderer::Init3DState()
{
    rlViewport(0,0,(RLint)mViewPort.GetWidth(),(RLint)mViewPort.GetHeight()); 
#if 0
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
#endif
}

RLRenderer::RLRenderer()
{
	mCamera=&Camera::DefaultCamera();
    context = OpenRLCreateContext(NULL, RLRenderer::errorCatcher, NULL);
    OpenRLSetCurrentContext(context);


	Init3DState();

	mVersion = std::string((const char*)rlGetString(RL_VERSION)); 
	mVendor = std::string((const char*)rlGetString(RL_VENDOR));
	mRenderer = std::string((const char*)rlGetString(RL_RENDERER));
	const char *ver = (const char*)rlGetString(RL_SHADING_LANGUAGE_VERSION);
	mShadingLanguageVersion = std::string(ver);
	char delim = ' ';
	std::string extString((const char *) rlGetString(RL_EXTENSIONS));
    if (extString.length())
	    mExtensions = StringUtil::Explode(extString, delim); 
    else
    {
        mExtensions = RLExtensionList();
    }

}
RLRenderer::~RLRenderer()
{
        OpenRLDestroyContext(context);
}

bool RLRenderer::Update()
{
	return true;
}


void RLRenderer::PreRender()
{
#if 0
	glClearColor(0.0, 0.0, 1.0, 0.0); glErrorCheck();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); glErrorCheck();
#endif
}
void RLRenderer::PostRender()
{
}

template<typename VertexType>
void DrawNormals(const VertexData &data)
{
#if 0
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
#endif
}
void RLRenderer::Render(const RenderObjectList &renderObjectList)
{
#if 0
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
#endif
}

void RLRenderer::Render(const RenderObject2DList &renderObjectList)
{
#if 0
    Matrix44 ortho;
    ortho.SetOrtho(0.0, (Real)viewportHeight, (Real)viewportWidth, 0.0, -1.0, 1.0);
#ifdef ENABLE_FIX_PIPELINE
    SetTransform(ortho, Matrix44::IDENTITY, Matrix44::IDENTITY);
#else
    Shader shader;
    shader.Bind();
    SetTransform(proj, Matrix44::IDENTITY, Matrix44::IDENTITY, &shader);
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
#endif
}

void RLRenderer::Render(const Text2DList &renderObjectList)
{
#if 0
    Matrix44 ortho;
    ortho.SetOrtho(0.0, (Real)viewportHeight, (Real)viewportWidth, 0.0, -1.0, 1.0);
#ifdef ENABLE_FIX_PIPELINE
    SetTransform(ortho, Matrix44::IDENTITY, Matrix44::IDENTITY);
#else
    Shader shader;
    shader.Bind();
    SetTransform(proj, Matrix44::IDENTITY, Matrix44::IDENTITY, &shader);
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
#endif
}

static bool isRenderingExtra = false;
void RLRenderer::RenderExtra(uint8_t *rgb_front, size_t width, size_t height, size_t depth)
{
#if 0
#if !defined(ES1) && !defined(ES2)
	glBindTexture(GL_TEXTURE_2D, gl_depth_tex);glErrorCheck();
	if (depth == 1) {
		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)depth, (GLsizei)width, (GLsizei)height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, rgb_front); glErrorCheck();
	}
	else if (depth == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)depth, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_front); glErrorCheck();
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
#endif
}
void RLRenderer::Render(uint8_t *rgb_front, uint8_t *depth_front)
{
#if 0
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
#endif
}

void RLRenderer::RenderState::SetTextureEnabled(bool isEnabled)
{
#if 0
	if (isEnabled)
	{
		glEnable(GL_TEXTURE_2D); glErrorCheck();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);  glErrorCheck();
		glDisable(GL_TEXTURE_2D); glErrorCheck();
	}
#endif
}


Matrix44 RLRenderer::GetRLProjection()
{

    	Matrix44 projectionMatrixd;
GCLAssert(false);
	return projectionMatrixd;
}
Matrix44 RLRenderer::GetRLModelView()
{
    	Matrix44 modelViewMatrixd;
GCLAssert(false);
	return modelViewMatrixd;
}


void RLRenderer::SetTransform( const Matrix44 &projection, const Matrix44 &modelView, const Matrix44 &transform, Shader *shader)
{

	Matrix44 f = modelView*transform;


	if (shader)
	{
		shader->SetProjectionMatrix(projection);
		shader->SetModelViewMatrix(f);
	}
    else
    {
        GCLAssertMsg(false, "We need a shader");
    }

}
#endif
