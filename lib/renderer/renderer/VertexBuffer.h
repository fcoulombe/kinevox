#pragma once
#include "renderer/OpenGL.h"
#include "renderer/ShaderAttributeDefaultLocations.h"
#include "renderer/Vertex.h"

namespace GCL
{

  template<typename VertexType>
  class VertexBuffer
  {
  public:
    VertexBuffer(const VertexType *vertexArray, size_t count)
    : mBufferType(GL_STATIC_DRAW),
      mVertexCount(count)
    {
      glGenBuffers(1, &mVertexBufferId);glErrorCheck();
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);glErrorCheck();
      glBufferData(GL_ARRAY_BUFFER, sizeof(VertexType)*count, (void*)vertexArray, mBufferType);glErrorCheck();
    }


    ~VertexBuffer()
    {
      glDeleteBuffers(1, &mVertexBufferId);glErrorCheck();
    }
    void PreRender()
    {
      if (VertexType::GetComponentType() & ePOSITION)
        {
          glVertexAttribPointer(ATTRIB_POSITION, 3, GL_DOUBLE, GL_FALSE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToPosition());glErrorCheck();
          glEnableVertexAttribArray(ATTRIB_POSITION);glErrorCheck();
        }
      if (VertexType::GetComponentType() & eNORMAL)
        {
          glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_TRUE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToNormal());glErrorCheck();
          glEnableVertexAttribArray(ATTRIB_NORMAL);glErrorCheck();
        }
      if (VertexType::GetComponentType() & eTEXTURE_COORD)
        {
          glVertexAttribPointer(ATTRIB_TEXTURE_COORD, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), (char*)NULL+VertexType::OffsetToTextureCoordinate());glErrorCheck();
          glEnableVertexAttribArray(ATTRIB_TEXTURE_COORD);glErrorCheck();
        }
    }

    void Render()
    {
      PreRender();
      glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertexCount);glErrorCheck();
      PostRender();
    }

    void PostRender()
    {
      if (VertexType::GetComponentType() & ePOSITION)
        {
          glDisableVertexAttribArray(ATTRIB_POSITION);glErrorCheck();
        }
      if (VertexType::GetComponentType() & eNORMAL)
        {
          glDisableVertexAttribArray(ATTRIB_NORMAL);glErrorCheck();
        }
      if (VertexType::GetComponentType() & eTEXTURE_COORD)
        {
          glDisableVertexAttribArray(ATTRIB_TEXTURE_COORD);glErrorCheck();
        }
    }

    bool IsValid() const { return mVertexBufferId!=(GLuint)-1; }

  private:
    GLuint mVertexBufferId;
    GLuint mBufferType;
    size_t mVertexCount;
  };

}
