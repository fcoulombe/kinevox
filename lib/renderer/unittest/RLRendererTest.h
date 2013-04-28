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
#pragma once

#include <kinetestlib/UnitTest.h>
#include <renderer/RL/RLRenderer.h>
#include <renderer/RL/RLTexture.h>
#include <renderer/RL/RLFrameBuffer.h>
#include <renderer/RL/RLProgram.h>
#include <renderer/RL/RLShader.h>
#include <renderer/RL/RLVertexBuffer.h>
#include <windriver/WinDriver.h>
#include <renderer/TextureResourceManager.h>
#include <renderer/GeomUtil.h>
using namespace GCL;
namespace RLRendererTest
{
    
    void Test();
void Test()
{
	TEST_START
        	TextureResourceManager::Initialize();
    {
#if 0
    //WinDriver winDriver("GLRendererTest");
	RLRenderer renderer;

    printf("bleh");
	std::cout << "OpenGL Stats"<<std::endl;
	std::cout << "Version: " << renderer.GetVersion()<<std::endl;
	std::cout << "Vendor: " << renderer.GetVendor()<<std::endl;
	std::cout << "Renderer: " << renderer.GetRenderer()<<std::endl;
	std::cout << "ShadingLanguageVersion: " << renderer.GetShadingLanguageVersion()<<std::endl;

	std::cout << "Extensions: " << std::endl;
	const std::vector<std::string> &ext = renderer.GetExtensions();
	for (size_t i=0; i<ext.size();++i)
	{
		std::cout << ext[i] << std::endl;;
	}


	//test default viewport values
	{
		std::stringstream s;
		s<<renderer.GetViewPort().GetHeight()<<"=="<<Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT");
		AssertMsg_Test(renderer.GetViewPort().GetHeight()==Config::Instance().GetInt("DEFAULT_VIEWPORT_HEIGHT"), s.str().c_str());
	}
	{
		std::stringstream s;
		s<<renderer.GetViewPort().GetWidth()<<"=="<<Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH");
		AssertMsg_Test(renderer.GetViewPort().GetWidth()==Config::Instance().GetInt("DEFAULT_VIEWPORT_WIDTH"), s.str().c_str());
	}

    RLTexture mush(TEXTURE_PATH"mushroomtga.tga");
    mush.Save("RLMushroom.tga");

    RLTexture colorBuffer(renderer.GetViewPort().GetWidth(),renderer.GetViewPort().GetHeight());
    RLFrameBuffer frameBuffer(colorBuffer);
    frameBuffer.Bind();
    
    RLProgram frameProgram;
    RLShader frameShader("DefaultFrameShader.txt", RL_FRAME_SHADER);
    frameProgram.AttachShader(frameShader);
    frameProgram.Link();
    frameProgram.Bind();
#if 1
    RLProgram geomProgram;
    RLShader vertexShader("DefaultVertexShader.txt", RL_VERTEX_SHADER);
    RLShader rayShader("DefaultRayShader.txt", RL_RAY_SHADER);
    geomProgram.AttachShader(vertexShader);
    geomProgram.AttachShader(rayShader);
    geomProgram.Link();
    geomProgram.Bind();
    AttribLocations loc;
    loc.position = geomProgram.GetAttributeLocation("InPosition");

    std::vector<WorldPoint3> vertexData;
    std::vector<WorldPoint3> normalData;
    std::vector<WorldPoint2> tcoordData;
    GeomUtil::MakeMeshCube(vertexData,normalData,tcoordData,1.0);
    float transformMatrix[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, -3.0f, 1.0f,
    };


    RLVertexBuffer<VertexP> buffer((const VertexP*)vertexData.data(), vertexData.size());
    rlPrimitiveParameterMatrixf(RL_PRIMITIVE, RL_PRIMITIVE_TRANSFORM_MATRIX, transformMatrix);
    buffer.BindShaderLoc(loc);
    buffer.Render();
    
#endif
    //frameProgram.Bind();
    rlRenderFrame();

    colorBuffer.Save("RLColorBuffer.tga");
#else
    {
        OpenRLContext context = OpenRLCreateContext(NULL, RLRenderer::errorCatcher, NULL);
        OpenRLSetCurrentContext(context);
        rlViewport(0,0,640,480); 
        RLtexture mTextureId;
        rlGenTextures(1, &mTextureId); 
        rlBindTexture(RL_TEXTURE_2D, mTextureId);
        rlTexImage2D(RL_TEXTURE_2D, 0, RL_RGBA,
            640, 480, 0,RL_RGBA,
            RL_UNSIGNED_BYTE, NULL);

        RLframebuffer mainFramebuffer;
        rlGenFramebuffers(1, &mainFramebuffer);
        rlBindFramebuffer(RL_FRAMEBUFFER, mainFramebuffer);
        rlFramebufferTexture2D(RL_FRAMEBUFFER, RL_COLOR_ATTACHMENT0,
            RL_TEXTURE_2D, mTextureId, 0);
#if 0
        const char *shaderSource = "void main()\n"
        "{\n"
            "vec4 color = vec4(rl_FrameCoord.x/rl_FrameSize.x,\n"
                "rl_FrameCoord.y/rl_FrameSize.y,\n"
                "0.0, 1.0);\n"
                "	if(rl_FrameCoord.x<1.0)\n"
                "       print(rl_FrameCoord);;\n"
            "accumulate(color);\n"
        "}\n";
#else
        const char *shaderSource = "void setup()"
        "{"
            "rl_OutputRayCount = 1;"
        "}"
        "void main()"
        "{"
            "vec3 origin = vec3(0.0);"
            "float frameScale = 3.0;"
            "vec2 frameSize = rl_FrameSize.xy;"
            "vec2 normalFramePosition = rl_FrameCoord.xy/frameSize;"
            "vec3 viewportPosition = vec3(frameScale*(normalFramePosition - 0.5), -3.0);"
            "createRay();"
            "rl_OutRay.origin = origin;"
            "rl_OutRay.direction = viewportPosition - origin;"
            "emitRay();"
        "}";
#endif
        RLshader frameShader;
        frameShader = rlCreateShader(RL_FRAME_SHADER);
        rlShaderSource(frameShader, 1, &shaderSource, NULL);
        rlCompileShader(frameShader);
        int compileStatus;
        rlGetShaderiv(frameShader, RL_COMPILE_STATUS, &compileStatus);
        if(compileStatus == RL_FALSE)
        {
            const char* log;
            rlGetShaderString(frameShader, RL_COMPILE_LOG, &log);
            printf("Failed to compile frame shader:\n%s\n", log);
            exit(1);
        }
        RLprogram frameProgram;
        frameProgram = rlCreateProgram();
        rlAttachShader(frameProgram, frameShader);
        rlLinkProgram(frameProgram);
        int linkStatus;
        rlGetProgramiv(frameProgram, RL_LINK_STATUS, &linkStatus);
        if(linkStatus == RL_FALSE)
        {
            const char* log;
            // Get the link errors and print them out.
            rlGetProgramString(frameProgram, RL_LINK_LOG, &log);
            printf("Failed to link program:\n%s\n", log);
            exit(1);
        }

        const char* g_basicVertexShaderSource = "attribute vec3 positionAttribute;"
        "void main()"
        "{"
        "    rl_Position = vec4(positionAttribute, 0.0);"
        "}";
        const char* g_triangleRayShaderSource = "void main()"
        "{"
            "vec3 color = vec3(0.8, 0.3, 0.01);"
            "accumulate(color);"
        "}";
        RLshader triangleRayShader;
        RLshader triangleVertexShader;
        RLprogram triangleProgram;
        triangleRayShader = rlCreateShader(RL_RAY_SHADER);
        rlShaderSource(triangleRayShader, 1, &g_triangleRayShaderSource, NULL);
        rlCompileShader(triangleRayShader);

        rlGetShaderiv(triangleRayShader, RL_COMPILE_STATUS, &compileStatus);
        if(compileStatus == RL_FALSE)
        {
            const char* log;
            rlGetShaderString(triangleRayShader, RL_COMPILE_LOG, &log);
            printf("Failed to compile frame shader:\n%s\n", log);
            exit(1);
        }

        triangleVertexShader = rlCreateShader(RL_VERTEX_SHADER);
        rlShaderSource(triangleVertexShader, 1, &g_basicVertexShaderSource, NULL);
        rlCompileShader(triangleVertexShader);

        rlGetShaderiv(triangleVertexShader, RL_COMPILE_STATUS, &compileStatus);
        if(compileStatus == RL_FALSE)
        {
            const char* log;
            rlGetShaderString(triangleVertexShader, RL_COMPILE_LOG, &log);
            printf("Failed to compile frame shader:\n%s\n", log);
            exit(1);
        }

        triangleProgram = rlCreateProgram();
        rlAttachShader(triangleProgram, triangleRayShader);
        rlAttachShader(triangleProgram, triangleVertexShader);
        rlLinkProgram(triangleProgram);

        rlGetProgramiv(triangleProgram, RL_LINK_STATUS, &linkStatus);
        if(linkStatus == RL_FALSE)
        {
            const char* log;
            // Get the link errors and print them out.
            rlGetProgramString(triangleProgram, RL_LINK_LOG, &log);
            printf("Failed to link program:\n%s\n", log);
            exit(1);
        }


        RLprimitive trianglePrimitive;
        rlGenPrimitives(1, &trianglePrimitive);
        rlBindPrimitive(RL_PRIMITIVE, trianglePrimitive);
        rlUseProgram(triangleProgram);

        float vertexPositions[3*3] = {
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
        };
        RLbuffer vertexPositionsBuffer;
        rlGenBuffers(1, &vertexPositionsBuffer);
        rlBindBuffer(RL_ARRAY_BUFFER, vertexPositionsBuffer);
        rlBufferData(RL_ARRAY_BUFFER, 3*3*sizeof(float), vertexPositions, RL_STATIC_DRAW);

        RLint attribLocation = rlGetAttribLocation(triangleProgram, "positionAttribute");
        rlVertexAttribBuffer(attribLocation, 3, RL_FLOAT, RL_FALSE, 0, 0);

        typedef struct Vector3
        {
            float x;
            float y;
            float z;
        } Vector3;
        // All objects in the scene will be translated by this amount
        // so they will be in front of the camera.
        const Vector3 g_sceneTranslation = {0.0f, 0.0f, -3.0f};
        float transformMatrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            g_sceneTranslation.x, g_sceneTranslation.y, g_sceneTranslation.z, 1.0f,
        };
        rlPrimitiveParameterMatrixf(RL_PRIMITIVE, RL_PRIMITIVE_TRANSFORM_MATRIX, transformMatrix);

        rlDrawArrays(RL_TRIANGLES, 0, 3);



        //rlBindPrimitive(RL_PRIMITIVE, (RLprimitive)NULL);
        //rlUseProgram(frameProgram);
        //rlRenderFrame();
        
        RLbuffer pixelBuffer;
        rlGenBuffers(1, &pixelBuffer);
        rlBindBuffer(RL_PIXEL_PACK_BUFFER, pixelBuffer);
        rlBufferData(RL_PIXEL_PACK_BUFFER,
            640*480*4*sizeof(uint8_t),
            0,
            RL_STATIC_DRAW);

        rlBindBuffer(RL_PIXEL_PACK_BUFFER, pixelBuffer);
        rlBindTexture(RL_TEXTURE_2D, mTextureId);
        rlGetTexImage(RL_TEXTURE_2D, 0, RL_RGBA, RL_UNSIGNED_BYTE, NULL);
        uint8_t *pixels = (uint8_t*)rlMapBuffer(RL_PIXEL_PACK_BUFFER, RL_READ_ONLY);
        PixelBuffer::SaveTga("RLColorBuffer.tga", 640,480,4,pixels);

        // We no longer need access to the original pixels
        rlUnmapBuffer(RL_PIXEL_PACK_BUFFER);
    }
#endif
    }
    TextureResourceManager::Terminate();

}
}
