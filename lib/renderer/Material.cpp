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

#include "renderer/Material.h"

#include <fstream>
#include <sstream>

#include <gcl/Assert.h>
#include <gcl/Config.h>
#include <gcl/Log.h>
#include <gcl/StringUtil.h>
#include "renderer/GPUProgram.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

using namespace GCL;

//TODO: turn this into a material reosurce
Material::Material(const char *filename)
{
	mVertexShader = NULL;
	mFragmentShader = NULL;
	mProgram = NULL;
	mTexture=NULL;
	LoadMaterial(filename);
}

Material::~Material()
{
	delete mFragmentShader;
	delete mVertexShader;
	delete mProgram;
	delete mTexture;
}

void Material::LoadMaterial(const char *filename)
{
	KLog("LoadMaterial %s", filename);
	if (mProgram )
	{
		delete mFragmentShader;
		delete mVertexShader;
		delete mProgram;
		mProgram = NULL;
		mVertexShader = NULL;
		mFragmentShader = NULL;
	}
	if (mTexture)
	{
		delete mTexture;
		mTexture=NULL;
	}
	std::string fullFileName(std::string(MATERIAL_PATH) + std::string(filename) + std::string(".material"));
	std::vector<std::string> resourcePathList;
	if (Config::Instance().HasString("RESOURCE_PATH"))
	{
		const std::string &resourcePaths = Config::Instance().GetString("RESOURCE_PATH");
		StringUtil::Explode(resourcePaths, resourcePathList, ';');
	}
	for (size_t i=0; i<resourcePathList.size(); ++i)
	{
		std::string fullPath = resourcePathList[i] + "/" + fullFileName;
		if (GCLFile::Exists(fullPath.c_str()))
		{
			fullFileName = fullPath;
			break;
		}
	}
	GCLFile fp(fullFileName.c_str());
	auto fileBuffer = fp.ReadAll();
	std::string fileContent((const char *)fileBuffer.get());
	std::vector<std::string> lines;
	StringUtil::Explode(fileContent, lines, '\n');

	const size_t BUFFER_SIZE = 1024;
	char vshader[BUFFER_SIZE];
	char fshader[BUFFER_SIZE];
	sscanf(lines[0].c_str(), "vertex_shader: %s", vshader);
	sscanf(lines[1].c_str(), "frag_shader: %s", fshader);

	//shader: default
	char texture[BUFFER_SIZE];
	sscanf(lines[2].c_str(), "texture: %s", texture);
	//texture: TEXTURE_PATH/mushroomtga.tga

#if USE_64BIT_PLATFORM
#	define FLOAT_FLAG "%lf"
#else
#	define FLOAT_FLAG "%f"
#endif
	WorldPoint4 &ambient = mAmbient;
	sscanf(lines[3].c_str(), "ambient: " FLOAT_FLAG " " FLOAT_FLAG " " FLOAT_FLAG " " FLOAT_FLAG, &ambient.x, &ambient.y, &ambient.z, &ambient.w);

	WorldPoint4 &diffuse = mDiffuse;
	sscanf(lines[4].c_str(), "diffuse: " FLOAT_FLAG " " FLOAT_FLAG " " FLOAT_FLAG " " FLOAT_FLAG, &diffuse.x, &diffuse.y, &diffuse.z, &diffuse.w);

	WorldPoint4 &specular = mSpecular;
	sscanf(lines[5].c_str(), "specular: " FLOAT_FLAG " " FLOAT_FLAG " " FLOAT_FLAG " " FLOAT_FLAG, &specular.x, &specular.y, &specular.z, &specular.w);

	WorldPoint4 &emissive = mEmissive;
	sscanf(lines[6].c_str(), "emissive: " FLOAT_FLAG " " FLOAT_FLAG " " FLOAT_FLAG " " FLOAT_FLAG, &emissive.x, &emissive.y, &emissive.z, &emissive.w);

	Real &shininess = mShininess;
	sscanf(lines[7].c_str(), "shininess: " FLOAT_FLAG, &shininess);

	fp.Close();

	mVertexShader = new Shader(vshader, VERTEX_SHADER);
	mFragmentShader =  new Shader(fshader, FRAGMENT_SHADER);
	mProgram = new GPUProgram(*mVertexShader, *mFragmentShader);
    std::stringstream s;
    s<< "Material: "<<filename<< " doesn't have a texture. for now all materials must have a texture"<<std::endl;
    GCLAssertMsg(strcmp(texture, "null")!=0, s.str().c_str());
	mTexture = new Texture(texture);
}


void Material::Bind() const
{
	mProgram->Bind();
	mTexture->Bind();
}

void Material::SetTexture(const char *texture)
{
	if (mTexture)
		delete mTexture;
	mTexture = new Texture(texture);
}
