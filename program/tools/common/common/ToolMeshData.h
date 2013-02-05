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

#pragma once

#include <vector>
#include <gcl/Point4.h>
#include "common/BufferWriter.h"
#include "common/ToolMaterialData.h"

namespace GCL
{

    struct ToolMeshDataBak
    {
        ToolMeshDataBak()
            :mVertexCount(0),
            mIndicesCount(0),
            mNormalCount(0),
            mVertexColorCount(0),
            mMaterialCount(0),
            mUvCount(0)
        {}

        uint32_t mVertexCount;
        uint32_t mIndicesCount;
        uint32_t mNormalCount;
        uint32_t mVertexColorCount;
        uint32_t mMaterialCount;
        uint32_t mUvCount;

        std::vector<WorldPoint4> mVertexList;
        std::vector<int> mIndiceList;
        std::vector<WorldPoint3> mNormalList;
        std::vector<WorldPoint4> mVertexColor;
        std::vector<WorldPoint2> mUvList;

        ToolMaterialData mMaterialData;
    };

    struct ToolSubMeshData
    {
        ToolSubMeshData()
        {}
        std::vector<WorldPoint3> mVertexList;
        std::vector<int> mIndiceList;
        std::vector<WorldPoint3> mNormalList;
        std::vector<WorldPoint4> mVertexColor;
        std::vector<WorldPoint2> mUvList;
    };

    GCLINLINE BufferWriter & operator<<( BufferWriter& buffer, const ToolSubMeshData &meshData)
    {
        size_t startOffset = buffer.GetCurrentOffset();
        uint32_t toWrite;
        toWrite = 0xbaadf00d;
        uint32_t *meshSize = (uint32_t *)(buffer.GetBuffer()+startOffset);
        buffer.Write(toWrite);
        toWrite = meshData.mVertexList.size();
        buffer.Write(toWrite);
        toWrite = meshData.mIndiceList.size();
        buffer.Write(toWrite);
        for (size_t i=0; i<meshData.mIndiceList.size(); ++i)
        {
            uint32_t indice = meshData.mIndiceList[i];
            buffer.Write(indice);
        }
        for (size_t i=0; i<meshData.mIndiceList.size(); ++i)
        {
            int indice = meshData.mIndiceList[i];

            const WorldPoint3 &tempVert = meshData.mVertexList[indice];
            buffer.Write(tempVert.x);
            buffer.Write(tempVert.y);
            buffer.Write(tempVert.z);

            const WorldPoint3 &tempNormal = meshData.mNormalList[indice];
            buffer.Write(tempNormal.x);
            buffer.Write(tempNormal.y);
            buffer.Write(tempNormal.z);

            const WorldPoint2 &tempUv = meshData.mUvList[i];
            buffer.Write(tempUv.x);
            buffer.Write(tempUv.y);
        }
        size_t endOffset = buffer.GetCurrentOffset();
        *meshSize = endOffset-startOffset;
        return buffer;
    }
    struct ToolMeshData
    {
        ToolMeshData()
            :mMaterialCount(0)
        {}

        uint32_t mMaterialCount;
        ToolMaterialData mMaterialData;
        std::vector<ToolSubMeshData> mSubMeshList;

    };
    GCLINLINE BufferWriter & operator<<( BufferWriter& buffer, const ToolMeshData &meshData)
    {
        uint32_t toWrite = meshData.mSubMeshList.size();
        buffer.Write(toWrite);
        buffer.Write(meshData.mMaterialCount);
        buffer << meshData.mMaterialData.matName;
        for (size_t i=0; i<meshData.mSubMeshList.size(); ++i)
        {
            buffer<<meshData.mSubMeshList[i];
        }
        //buffer << meshData.mMaterialData ;

        return buffer;
    }
    GCLINLINE std::ostream& operator<<( std::ostream& output, const ToolMeshData &meshData)
    {
        //output << "PolyVertexCount: " << meshData.mVertexCount<<std::endl;
        output << "MaterialCount: " << meshData.mMaterialCount <<std::endl;
        //output << "ElemNormCount: " << meshData.mNormalCount <<std::endl;
        //output << "UvCount: " << meshData.mUvCount <<std::endl;

        /*
        output << "Normals"<< std::endl;
        for (size_t i=0; i<meshData.mNormalList.size(); ++i)
        {
        const WorldPoint3 &normal = meshData.mNormalList[i];
        output << normal.x << " " << normal.y << " " << normal.z << std::endl;
        }

        output << "Position"<< std::endl;
        for (size_t i=0; i<meshData.mVertexList.size(); ++i)
        {
        const WorldPoint4 &position = meshData.mVertexList[i];
        output << position.x << " " << position.y << " " << position.z << std::endl;
        }

        output << "Indices"<< std::endl;
        for (size_t i=0; i<meshData.mIndiceList.size(); ++i)
        {
        int indice = meshData.mIndiceList[i];
        output << indice << std::endl;
        }
        */
        return output;
    }
}
