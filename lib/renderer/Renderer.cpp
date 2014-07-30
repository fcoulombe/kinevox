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

#include "renderer/Renderer.h"
#include "renderer/GPUProgram.h"

using namespace GCL;


template<typename VertexType>
void DrawNormals(const VertexData &data, const AttribLocations &loc)
{
	std::vector<Point3<MeshReal> > normalLines;
	const VertexType *vertexData = (const VertexType *)(data.mVertexData); 
	for (size_t i=0; i<data.mVertexCount; ++i)
	{
		const VertexType &vertex = vertexData[i];
		normalLines.push_back(vertex.position);
		normalLines.push_back(vertex.position + (vertex.normal*0.5));
	}
	const VertexP *pos = (const VertexP *)(normalLines.data());
	VertexData lineData(pos, normalLines.size(), VertexP::GetComponentType());
	VertexBuffer buffer((const VertexP *)lineData.mVertexData, lineData.mVertexCount, loc);
	buffer.Render(VBM_LINES);

}
