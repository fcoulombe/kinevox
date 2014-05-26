/*
 * Copyright (C) 2014 by Francois Coulombe
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
#include <sstream>

#include <gcl/UnitTest.h>
#include <applayer/Node.h>
#include <applayer/GCLApplication.h>

using namespace GCL;
namespace NodeTest
{

void Test()
{
	KINEVOX_TEST_START
	//GCLApplication::Initialize("NodeTest");
	{
		Node node("TestNode");
		Assert_Test(node.GetTransform() == Matrix44::IDENTITY);
		Assert_Test(node.GetName() == "TestNode");
		Assert_Test(node.GetParent() == nullptr);

		node.SetName("TestNode2");
		Assert_Test(node.GetName() == "TestNode2");
		node.SetPosition(0.0,0.0,-10.0);
		Assert_Test(node.GetPosition() == WorldPoint3(0.0,0.0,-10.0));

		Assert_Test(node.GetChildCount() == 0);
		Node *newNode = new Node("ChildNode");
		node.AddChildNode(*newNode);
		Assert_Test(node.GetChildCount() == 1);

		Node *childNode = node.FindNode("ChildNode");
		Assert_Test(childNode->GetTransform() == Matrix44::IDENTITY);

		node.DetachChild(*childNode);
		delete childNode;
		childNode = node.FindNode("ChildNode");
		Assert_Test(childNode == nullptr);
		Assert_Test(node.GetChildCount() == 0);

		newNode = new Node("ChildNode");
		node.AddChildNode(*newNode);
		node.DeleteChild(*newNode);
		childNode = node.FindNode("ChildNode");
		Assert_Test(childNode == nullptr);
		Assert_Test(node.GetChildCount() == 0);

		newNode = new Node("ChildNode");
		node.AddChildNode(*newNode);

		node.SetTransform(Matrix44::IDENTITY);
		Assert_Test(node.GetTransform() == Matrix44::IDENTITY);

		node.SetOrientation(90.0, 0.0, 0.0);
		Matrix44 rotX;
		rotX.SetRotationX(90.0);
		Assert_Test(node.GetTransform() == rotX);

		node.SetOrientation(0.0, 90.0, 0.0);
		Matrix44 rotY;
		rotY.SetRotationY(90.0);
		Assert_Test(node.GetTransform() == rotY);

		node.SetOrientation(0.0, 0.0, 90.0);
		Matrix44 rotZ;
		rotZ.SetRotationZ(90.0);
		Assert_Test(node.GetTransform() == rotZ);

	}

	//GCLApplication::Terminate();
}
}
