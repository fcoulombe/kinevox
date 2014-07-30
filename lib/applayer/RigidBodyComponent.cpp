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

#include "applayer/RigidBodyComponent.h"
#include "applayer/Actor.h"
#include <gcl/AABox.h>
#include <gcl/Sphere.h>
#include <physics/RigidBody.h>

using namespace GCL;



RigidBodyComponent::RigidBodyComponent(Actor *parentActor, PtrLuaTableIterator &it)
	: Component(parentActor)
{
	mBody = nullptr;
	if (!it)
		return;
	Real weight = 0.0;
	AABox box;
	Sphere sphere;
	enum BodyShape
	{
		NONE,
		BOX,
		SPHERE
	};
	BodyShape shape = NONE;
	while (!it->End())
	{
		if (it->GetKey() == "weight")
		{
			weight = it->GetReal();
		}
		else if (it->GetKey() == "sphere")
		{
			shape = SPHERE;
			PtrLuaTableIterator sphereIt = it->GetTableIterator();
			while (!sphereIt->End())
			{
				if (sphereIt->GetKey() == "radius")
				{
					sphere.SetRadius(sphereIt->GetReal());
				}
				++(*sphereIt);
			}
		}
		else if (it->GetKey() == "box")
		{
			shape = BOX;
			PtrLuaTableIterator boxIt = it->GetTableIterator();

			Real width=0.;
			Real height=0.;
			Real length=0.;
			while (!boxIt->End())
			{
				if (boxIt->GetKey() == "width")
				{
					width = boxIt->GetReal();
				}
				else if (boxIt->GetKey() == "height")
				{
					height = boxIt->GetReal();
				}
				else if (boxIt->GetKey() == "length")
				{
					length = boxIt->GetReal();
				}
				++(*boxIt);
			}
			box = AABox(width, height, length);
			GCLAssert(box != AABox(0.,0.,0.));
		}

		++(*it);
	}
	switch (shape)
	{
	case BOX:
		mBody = new RigidBody(box, weight);
		break;
	case SPHERE:
		mBody = new RigidBody(sphere, weight);
		break;
	default:
		mBody = nullptr;
	}
}


RigidBodyComponent::~RigidBodyComponent()
{
	delete mBody;
}

void GCL::RigidBodyComponent::Update( Real )
{
	Matrix44 transform;
	mBody->GetTransform(transform);
	mParentActor->SetTransform(transform);
}

void RigidBodyComponent::SetPosition( const WorldPoint3 &position )
{
	GCLAssert(mBody);
	mBody->SetPosition(position);
}
void RigidBodyComponent::SetBody(const AABox &box, Real weight)
{
	GCLAssert(mBody == nullptr);
	mBody = new RigidBody(box, weight);
}

void RigidBodyComponent::SetBody(const Sphere &sphere, Real weight)
{
	GCLAssert(mBody == nullptr);
	mBody = new RigidBody(sphere, weight);
}
