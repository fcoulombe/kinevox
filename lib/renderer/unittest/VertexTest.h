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

#include <gcl/UnitTest.h>
#include <renderer/Vertex.h>

using namespace GCL;
namespace VertexTest
{
    void Test();
void Test()
{
	TEST_START

	//#pragma clang diagnostic push
	//#pragma clang diagnostic ignored "-Wunused-variable"
	{
		VertexP p;
		std::stringstream s;
		s<< p.GetComponentType() << "==ePOSITION";
		AssertMsg_Test(p.GetComponentType()==ePOSITION, s.str().c_str());

		try
		{
			p.OffsetToNormal();
			Assert_Test( false);
		}
		catch (GCLException & /*e*/)
		{
		}

		try
		{
			p.OffsetToTextureCoordinate();
			Assert_Test(false);
		}
		catch (GCLException & /*e*/)
		{
		}

		s.str("");
		s<< p.OffsetToPosition() <<"==0";
		AssertMsg_Test(p.OffsetToPosition() == 0,s.str().c_str());
	}

	{
		VertexPT pt;
		std::stringstream s;
		s<< pt.GetComponentType() << "==ePOSITION|eTEXTURE_COORD";
		AssertMsg_Test(pt.GetComponentType()==(ePOSITION|eTEXTURE_COORD), s.str().c_str());

		try
		{
			pt.OffsetToNormal();
			Assert_Test(false);
		}
		catch (GCLException & /*e*/)
		{
		}

		s.str("");
		s<< pt.OffsetToPosition() <<"==0";
		AssertMsg_Test(pt.OffsetToPosition() == 0,s.str().c_str());

		s.str("");
		s<< pt.OffsetToTextureCoordinate() <<"==sizeof(WorldPoint3)";
		AssertMsg_Test(pt.OffsetToTextureCoordinate() == sizeof(WorldPoint3),s.str().c_str());
	}
	{
		VertexPNT pnt;
		std::stringstream s;
		s<< pnt.GetComponentType() << "==ePOSITION|eNORMAL|eTEXTURE_COORD";
		AssertMsg_Test(pnt.GetComponentType()==(ePOSITION|eNORMAL|eTEXTURE_COORD), s.str().c_str());


		s.str("");
		s<< pnt.OffsetToNormal() <<"==sizeof(WorldPoint3)";
		AssertMsg_Test(pnt.OffsetToNormal() == sizeof(WorldPoint3),s.str().c_str());

		s.str("");
		s<< pnt.OffsetToPosition() <<"==0";
		AssertMsg_Test(pnt.OffsetToPosition() == 0,s.str().c_str());

		s.str("");
		s<< pnt.OffsetToTextureCoordinate() <<"==sizeof(WorldPoint3)+sizeof(WorldPoint3)";
		AssertMsg_Test(pnt.OffsetToTextureCoordinate() == sizeof(WorldPoint3)+sizeof(WorldPoint3),s.str().c_str());

	}
	{
		VertexPN pn;
		std::stringstream s;
		s<< pn.GetComponentType() << "==ePOSITION|eNORMAL";
		AssertMsg_Test(pn.GetComponentType()==(ePOSITION|eNORMAL), s.str().c_str());

		try
		{
			pn.OffsetToTextureCoordinate();
			Assert_Test(false);
		}
		catch (GCLException & /*e*/)
		{
		}

		s.str("");
		s<< pn.OffsetToPosition() <<"==0";
		AssertMsg_Test(pn.OffsetToPosition() == 0,s.str().c_str());

		s.str("");
		s<< pn.OffsetToNormal() <<"==sizeof(WorldPoint3)";
		AssertMsg_Test(pn.OffsetToNormal() == sizeof(WorldPoint3),s.str().c_str());

	}
	//#pragma clang diagnostic pop
}
}
