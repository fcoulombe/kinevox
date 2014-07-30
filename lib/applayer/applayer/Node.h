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

#include <list>
#include <gcl/Assert.h>
#include <gcl/Matrix44.h>

namespace GCL
{
struct NodeData
{
	uint32_t mId;
	uint32_t mNameLength;
	const char *GetName() const { return (const char*)(((uint8_t*)this) + sizeof(mId) + sizeof(mNameLength)); }
	const Matrix44 &GetTransform() const
	{
		return *(const Matrix44*)(((uint8_t*)this) + GetTransformOffset());
	}
	uint32_t GetNumChilds() const { return *(uint32_t*)(((uint8_t*)this) + sizeof(mId) + sizeof(mNameLength) + mNameLength + sizeof(Matrix44) + sizeof(uint32_t)); }
	const NodeData *GetChild(size_t index) const { return (const NodeData *)(((uint8_t*)this) + GetChildOffset(index)); }
private:
	uint32_t GetTransformOffset() const { return sizeof(mId) + sizeof(mNameLength) + mNameLength; }
	uint32_t GetChildOffset(size_t index) const { return GetChildOffsetTable()[index]; }
	uint32_t *GetChildOffsetTable() const { return (uint32_t*)(((uint8_t*)this) + sizeof(mId) + sizeof(mNameLength) + mNameLength + sizeof(Matrix44) + sizeof(uint32_t) + sizeof(uint32_t)); }
};
class Node
{
public:
	Node();
	Node(const std::string &name, Node *parent = nullptr);
	Node(const NodeData *data, Node *parent = nullptr);
	virtual ~Node();
	uint32_t GetId() const { return mId; }
	virtual void Update(Real dt)
	{
		for (auto it = mChilds.begin(); it != mChilds.end(); ++it)
		{
			Node *tempNode = *it;
			tempNode->Update(dt);
		}
	}
	void SetName(const std::string &name) { mName = name; }
	const std::string &GetName() const { return mName; }

	void SetParent(Node *parent) { mParentNode = parent; }
	Node *GetParent() { return mParentNode; }
	const Node *GetParent() const { return mParentNode; }

	void GCL::Node::AddChildNode( Node &newNode)
	{
		Node *oldParentNode = newNode.GetParent();
		if (oldParentNode)
			oldParentNode->DetachChild(newNode);
		newNode.SetParent(this);
		mChilds.push_back(&newNode);	
	}
	void DeleteChild(Node &child);
	void DetachChild(Node &child);
	size_t GetChildCount() const { return mChilds.size(); }
	Node *FindNode(const std::string &name)
	{
		for (auto it = mChilds.begin(); it != mChilds.end(); ++it)
		{
			Node *tempNode = *it;
			if (tempNode->GetName() == name)
			{
				return tempNode;
			}
		}
		return nullptr;
	}

	const Matrix44 &GetTransform() const {return mTransform; }
	void SetTransform(const Matrix44 &transform) {mTransform = transform; }

	void SetOrientation(Real x,Real y,Real z);
	const WorldPoint3 &GetPosition() const { return (const WorldPoint3&)(mTransform.GetPosition()); }
	virtual void SetPosition(Real x, Real y,Real z)
	{ SetPosition(WorldPoint3(x,y,z));	}
	virtual void SetPosition(const WorldPoint3 &position)
	{
		mTransform.SetPosition(position);
	}

protected:
	uint32_t mId;
	Matrix44 mTransform;
	Node *mParentNode;
	typedef std::list<Node*> NodeList;
	NodeList mChilds;
	std::string mName;
};

}
