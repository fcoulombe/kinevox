#include "renderer/Material.h"

#include <fstream>
#include <sstream>

#include "renderer/Shader.h"
#include "renderer/Texture.h"

using namespace GCL;

//TODO: turn this into a material reosurce
Material::Material(const char *filename)
{
	mShader = NULL;
	mTexture=NULL;
	LoadMaterial(filename);
}

void Material::LoadMaterial(const char *filename)
{
	if (mShader )
	{
		delete mShader;
		mShader = NULL;
	}
	if (mTexture)
	{
		delete mTexture;
		mTexture=NULL;
	}
	const std::string fullFileName(std::string(MATERIAL_PATH) + std::string(filename) + std::string(".mat"));
	std::ifstream fp(fullFileName.c_str());
	std::stringstream s;
	s << "Failed loading the file: " << fullFileName;
	GCLAssertMsg(fp.good(), s.str().c_str());


	const size_t BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	char shader[BUFFER_SIZE];
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "shader: %s", shader);

	//shader: default
	char texture[BUFFER_SIZE];
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "texture: %s", texture);
	//texture: TEXTURE_PATH/mushroomtga.tga

#if USE_64BIT_PLATFORM
#define FLOAT_FLAG "%lf"
#else
#define FLOAT_FLAG "%f"
#endif
	WorldPoint4 ambient;
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "ambient: "FLOAT_FLAG" "FLOAT_FLAG" "FLOAT_FLAG" "FLOAT_FLAG, &ambient.x, &ambient.y, &ambient.z, &ambient.w);

	WorldPoint4 diffuse;
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "diffuse: "FLOAT_FLAG" "FLOAT_FLAG" "FLOAT_FLAG" "FLOAT_FLAG, &diffuse.x, &diffuse.y, &diffuse.z, &diffuse.w);

	WorldPoint4 specular;
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "specular: "FLOAT_FLAG" "FLOAT_FLAG" "FLOAT_FLAG" "FLOAT_FLAG, &specular.x, &specular.y, &specular.z, &specular.w);

	WorldPoint4 emissive;
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "emissive: "FLOAT_FLAG" "FLOAT_FLAG" "FLOAT_FLAG" "FLOAT_FLAG, &emissive.x, &emissive.y, &emissive.z, &emissive.w);

	Real shininess;
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "shininess: "FLOAT_FLAG, &shininess);


	fp.close();


	mShader = new Shader();
	mTexture = new Texture(texture);


}


void Material::Bind() const
{
#if 1//!ENABLE_FIX_PIPELINE
	mShader->Bind();
#endif
	mTexture->Bind();
}

void Material::SetTexture(Texture &texture)
{
	if (mTexture)
		delete mTexture;
	mTexture = &texture;
}
