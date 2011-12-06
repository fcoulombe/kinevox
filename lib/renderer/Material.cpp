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
	const std::string fullFileName(std::string("data/") + std::string(filename) + std::string(".material"));
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
	//texture: data/mushroom.tga


	WorldPoint4 ambient;
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "ambient: %lf %lf %lf %lf", &ambient.x, &ambient.y, &ambient.z, &ambient.w);

	WorldPoint4 diffuse;
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "diffuse: %lf %lf %lf %lf", &diffuse.x, &diffuse.y, &diffuse.z, &diffuse.w);

	WorldPoint4 specular;
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "specular: %lf %lf %lf %lf", &specular.x, &specular.y, &specular.z, &specular.w);

	WorldPoint4 emissive;
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "emissive: %lf %lf %lf %lf", &emissive.x, &emissive.y, &emissive.z, &emissive.w);

	Real shininess;
	fp.getline(buffer, BUFFER_SIZE);
	sscanf(buffer, "shininess: %lf", &shininess);

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
