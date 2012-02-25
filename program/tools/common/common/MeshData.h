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


namespace GCL
{
struct MeshData
{
	size_t mVertexCount;
	std::vector<WorldPoint4> mVertexList;

	size_t mNormalCount;
	std::vector<WorldPoint3> mNormalList;

	size_t mVertexColorCount;
	std::vector<WorldPoint3> mVertexColor;

	size_t mIndicesCount;
	std::vector<int> mIndiceList;

	size_t mMaterialCount;
};
GCLINLINE std::ostream& operator<<( std::ostream& output, const MeshData &meshData)
{
	output << "PolyVertexCount: " << meshData.mVertexCount<<std::endl;
	output << "MaterialCount: " << meshData.mMaterialCount <<std::endl;
	output << "ElemNormCount: " << meshData.mNormalCount <<std::endl;


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

	return output;
}
}
