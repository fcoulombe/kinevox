#include "renderer/Shader.h"
#include <gcl/Exception.h>
#include <gcl/Point4.h>
#include <gcl/Matrix44.h>
#include "renderer/Texture.h"

using namespace GCL;



const char *PositionVShaderStr =
		"uniform mat4 ProjectionMatrix;\n"
		"uniform mat4 ModelViewMatrix;\n"
		"attribute vec4 InPosition;   \n"
		"void main()                 \n"
		"{                           \n"
		"   gl_Position = ProjectionMatrix * ModelViewMatrix * InPosition; \n"
		"}                           \n";


const char *PositionFShaderStr =
		"//precision mediump float;                   \n"
		"void main()                                \n"
		"{                                          \n"
		"  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); \n"
		"}                                          \n";

const char *TextureVShaderStr =
		"uniform mat4 ProjectionMatrix;\n"
		"uniform mat4 ModelViewMatrix;\n"
		"attribute vec4 InPosition;   \n"
		"attribute vec4 InNormal;   \n"
		"attribute vec4 InTexCoord;   \n"
		"varying vec2 texcoord;  \n"
		"varying vec4 color;  \n"
		"void main()                 \n"
		"{                           \n"
		"#ifdef ENABLE_FIX_PIPELINE \n"
		"	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex; \n"
		"	gl_TexCoord[0] = gl_MultiTexCoord0; \n"
		"#else \n"
		"   gl_Position = ProjectionMatrix * ModelViewMatrix * InPosition; \n"
		"	texcoord = InTexCoord.xy;\n"
		"#endif \n"
		"	color = InNormal; \n"
		"}                           \n";

const char *TextureFShaderStr =
		"//uniform float fade_factor;	\n"
		"uniform sampler2D texture; 	\n"
		"varying vec2 texcoord;	\n"
		"varying vec4 color;	\n"
		"void main() \n"
		"{\n"
		"#ifdef ENABLE_FIX_PIPELINE \n"
		"	gl_FragColor = texture2D( texture, gl_TexCoord[0].st ); \n"
		"#else \n"
		"	gl_FragColor = texture2D(texture, texcoord)+color;\n"
		"#endif \n"
		"//gl_FragColor = vec4(1.0,0.0,0.0,1.0);\n"
		"}\n";

const char *DefaultVShaderStr = TextureVShaderStr;
const char *DefaultFShaderStr = TextureFShaderStr;

const char *SHADER_HEADER =
		"#version 110	\n"
		"#pragma optimize(off) \n"
		"#pragma debug(on) \n"
#if ENABLE_FIX_PIPELINE
		"#define ENABLE_FIX_PIPELINE \n"
#endif
		"\n";

Shader::Shader()
:mIsValid (false)
{
	std::string vbuffer(SHADER_HEADER);
	vbuffer += DefaultVShaderStr;
	std::string fbuffer(SHADER_HEADER);
	fbuffer += DefaultFShaderStr;

	GLuint vertexShader = CompileShader(vbuffer.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShader = CompileShader(fbuffer.c_str(), GL_FRAGMENT_SHADER);

	mProgramObject = glCreateProgram();glErrorCheck();
	GCLAssertMsg(mProgramObject != 0, "Can't create program");

	glAttachShader(mProgramObject, vertexShader);glErrorCheck();
	glAttachShader(mProgramObject, fragmentShader);glErrorCheck();

	glBindAttribLocation(mProgramObject, 0, "vPosition");glErrorCheck();

	glLinkProgram(mProgramObject);glErrorCheck();

	GLint linked;
	glGetProgramiv(mProgramObject, GL_LINK_STATUS, &linked);glErrorCheck();
	if(linked)
	{
		glBindAttribLocation(mProgramObject, 0, "InPosition");
		glBindAttribLocation(mProgramObject, 1, "InNormal");
		glBindAttribLocation(mProgramObject, 2, "InTexCoord");
		mIsValid = true;
	}
	else
	{
		PrintInfoLog(mProgramObject);
		glDeleteProgram(mProgramObject);glErrorCheck();
		mIsValid = false;
	}

	return;
}

void Shader::Bind()
{
	GCLAssert(mIsValid);
	glUseProgram(mProgramObject);glErrorCheck();
}


GLuint Shader::CompileShader(const char *shaderSrc, GLenum type)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type); glErrorCheck();

	GCLAssert(shader != 0 && "glCreateShader(type);");

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL); glErrorCheck();

	// Compile the shader
	glCompileShader(shader); glErrorCheck();
	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled); glErrorCheck();

	if(!compiled)
	{
		GLint infologLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);glErrorCheck();
		if (infologLength > 0)
		{
			GLchar *infoLog = new GLchar[infologLength];
			int charsWritten  = 0;
			glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);

			std::string log = "Shader Compile Log: " ;
			log += infoLog;

			delete [] infoLog;

			throw GCLException(log);
		}
		glDeleteShader(shader);glErrorCheck();
		return 0;
	}
	return shader;
}

void Shader::PrintInfoLog(GLuint p)
{
	GLint infoLen = 0;
	glGetProgramiv(p, GL_INFO_LOG_LENGTH, &infoLen);glErrorCheck();
	if(infoLen > 1)
	{
		char* infoLog = new char[(sizeof(char) * infoLen)];
		glGetProgramInfoLog(p, infoLen, NULL, infoLog);glErrorCheck();
		std::cerr << "Error linking program:\n%s" << infoLog << std::endl;
		delete [] infoLog;
	}

}




void Shader::SetProjectionMatrix(const Matrix44 &m)
{
	GCLAssert(mIsValid);
	GLint projectionMatrixLoc = glGetUniformLocation(mProgramObject,"ProjectionMatrix");glErrorCheck();
	Matrix44f mf(m);
	glUniformMatrix4fv(projectionMatrixLoc,1,false,(const GLfloat*)&mf);glErrorCheck();
}
void Shader::SetModelViewMatrix(const Matrix44 &m)
{
	GCLAssert(mIsValid);
	GLint modelviewMatrixLoc = glGetUniformLocation(mProgramObject,"ModelViewMatrix");glErrorCheck();
	Matrix44f mm(m);
	glUniformMatrix4fv(modelviewMatrixLoc,1,false,(const GLfloat*)&mm);glErrorCheck();
}



void Shader::SetTextureSampler(const Texture &sampler)
{
	GLint textureLoc = glGetUniformLocation(mProgramObject,"texture");glErrorCheck();
	glUniform1i(textureLoc, sampler.GetTextureId());glErrorCheck();
}

void Shader::GetUniform(const char *uniformName, Matrix44 &m44) const
{
	GLfloat mf[16];
	GCLAssert(mIsValid);
	GLint uniformLoc = glGetUniformLocation(mProgramObject,uniformName);glErrorCheck();
	GCLAssert(uniformLoc!=-1);
	glGetUniformfv(	mProgramObject,uniformLoc,mf);glErrorCheck();
	m44 = Matrix44((const float *)mf);
}
void Shader::GetUniform(const char *uniformName, int &sampler) const
{
	GCLAssert(mIsValid);
	GLint uniformLoc = glGetUniformLocation(mProgramObject,uniformName);glErrorCheck();
	GCLAssert(uniformLoc!=-1);
	glGetUniformiv(mProgramObject,uniformLoc,&sampler);glErrorCheck();
}
int Shader::GetAttributeLocation(const char *attributeName) const
{
	int ret =  (int)glGetAttribLocation(mProgramObject,attributeName);glErrorCheck();
	GCLAssertMsg(ret!=-1, attributeName);
	return ret;
}
void Shader::ResetDefault()
{
	glUseProgram(0);glErrorCheck();
}
