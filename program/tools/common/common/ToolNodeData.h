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

#include <string>
#include <vector>
#include "common/BufferWriter.h"
#include <gcl/Matrix44.h>

namespace GCL
{
    struct ToolNodeData
    {
        ToolNodeData()
            : mName("NULL")
        {
            mParent = NULL;
        }
        uint32_t mId;
        std::string mName;
        Matrix44 mTransform;
        ToolNodeData *mParent;
        std::vector<ToolNodeData> mNodeList;
    };

    GCLINLINE void WriteNode(BufferWriter& buffer, ToolNodeData &node, ToolNodeData *parent, std::vector<uint32_t> &nodeOffset)
    {
        node.mId = (uint32_t)nodeOffset.size();
        buffer.Write(node.mId); //node ID
        nodeOffset.push_back((uint32_t)buffer.GetCurrentOffset());
        buffer << node.mName;
        buffer << node.mTransform;
        uint32_t parentOffset;
        if (parent == NULL)
        {
            parentOffset = (uint32_t)-1; //root node doesn't have a parent
        }
        else
        {
            //find the parent's offset
            parentOffset = nodeOffset[parent->mId];
        }
        buffer.Write(parentOffset);
        uint32_t numberOfChilds = (uint32_t)node.mNodeList.size(); 
        buffer.Write(numberOfChilds); // write the number of childs
        uint32_t *childList = (uint32_t *)&(buffer.GetBuffer()[buffer.GetCurrentOffset()]);
        const uint32_t emptyOffset = 0xbaadf00d;
        for (size_t i=0; i<node.mNodeList.size(); ++i)
        {
            buffer.Write(emptyOffset);
        }
        for (size_t i=0; i<node.mNodeList.size(); ++i)
        {
            childList[i] = (uint32_t)buffer.GetCurrentOffset(); //store the offset of the direct childs;
            WriteNode(buffer, node.mNodeList[i], &node, nodeOffset); //write the child hierarchy
        }
    }

    GCLINLINE BufferWriter & operator<<( BufferWriter& buffer, ToolNodeData &nodeData)
    {

        //
        // node list with offset
        //
        // node1 offset 0
        // node2 offset 10
        // node3 offset 20
        // 
        //offset 0
        // node1
        //  name
        //  transform
        //  parent
        //  child offset list
        std::vector<uint32_t> nodeOffset;
        WriteNode(buffer, nodeData, NULL, nodeOffset);




  /*      uint32_t toWrite = meshData.mSubMeshList.size();
        buffer.Write(toWrite);
        buffer.Write(meshData.mMaterialCount);
        buffer << meshData.mMaterialData.matName;
        for (size_t i=0; i<meshData.mSubMeshList.size(); ++i)
        {
            buffer<<meshData.mSubMeshList[i];
        }
        //buffer << meshData.mMaterialData ;
        */
        return buffer;
    }

}
