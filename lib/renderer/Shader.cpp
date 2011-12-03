#include "renderer/Shader.h"
#include <gcl/Exception.h>
using namespace GCL;



const char *PositionVShaderStr =
    "#version 110                \n"
    "uniform mat4 ProjectionModelviewMatrix;\n"
    "uniform mat4 ModelviewMatrix;\n"
    "attribute vec4 InVertex;   \n"
    "void main()                 \n"
    "{                           \n"
    "   gl_Position = ProjectionModelviewMatrix * InVertex; \n"
    "}                           \n";


const char *BlankFShaderStr =
    "#version 110                               \n"
    "//precision mediump float;                   \n"
    "void main()                                \n"
    "{                                          \n"
    "  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); \n"
    "}                                          \n";

const char *TextureVShaderStr =
    "#version 110                \n"
    "uniform mat4 ProjectionModelviewMatrix;\n"
    "uniform mat4 ModelviewMatrix;\n"
    "attribute vec4 InVertex;   \n"
	"attribute vec4 InTexCoord;   \n"
	"attribute vec4 InNormal;   \n"
	"varying vec2 texcoord;  \n"
    "void main()                 \n"
    "{                           \n"
    "   gl_Position = ProjectionModelviewMatrix * InVertex; \n"
	"	texcoord = InTexCoord.xy;\n"
    "}                           \n";

const char *TextureFShaderStr =
		"#version 110	\n"
		"//uniform float fade_factor;	\n"
		"uniform sampler2D textures[1]; 	\n"
		"varying vec2 texcoord;	\n"
		"void main() \n"
    	"{\n"
        	"//gl_FragColor = texture2D(textures[0], texcoord);\n"
			"gl_FragColor = vec4(1.0,0.0,0.0,1.0);\n"
    	"}\n";

const char *DefaultVShaderStr = TextureVShaderStr;
const char *DefaultFShaderStr = TextureFShaderStr;

Shader::Shader()
:mIsValid (false)
{
  GLuint vertexShader = CompileShader(DefaultVShaderStr, GL_VERTEX_SHADER);
  GLuint fragmentShader = CompileShader(DefaultFShaderStr, GL_FRAGMENT_SHADER);

  mProgramObject = glCreateProgram();glErrorCheck();
  GCLAssertMsg(mProgramObject != 0, "Can't create program");

  glAttachShader(mProgramObject, vertexShader);glErrorCheck();
  glAttachShader(mProgramObject, fragmentShader);glErrorCheck();

  glBindAttribLocation(mProgramObject, 0, "vPosition");glErrorCheck();

  glLinkProgram(mProgramObject);glErrorCheck();

  GLint linked;
  glGetProgramiv(mProgramObject, GL_LINK_STATUS, &linked);glErrorCheck();
  if(!linked)
    {
      PrintInfoLog(mProgramObject);
      glDeleteProgram(mProgramObject);glErrorCheck();
      mIsValid = false;
    }
  mIsValid = true;
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
