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

#include <vector>
#include <list>
#include <gcl/Assert.h>
#include <gcl/Matrix44.h>
#include "applayer/Component.h"
#include "applayer/Node.h"

namespace GCL
{
	enum ActorEvent
	{
		AE_REGISTER_RENDER_OBJECT
	};
class Actor : public Node
{
public:
	Actor(const char *name, const char *archetype);
	Actor(const char *name);

	~Actor();

	void Update(Real dt)
	{
		for (auto it = mComponentList.begin(); it != mComponentList.end(); ++it)
		{
			Component *tempComponent = it->second;
			tempComponent->Update(dt);
		}
	}
	void Render(const Matrix44 &proj)
	{
		for (auto it = mComponentList.begin(); it != mComponentList.end(); ++it)
		{
			Component *tempComponent = it->second;
			tempComponent->Render(proj);
		}
	}
	Component *GetComponent(const std::string &componentName)
	{
		Component *tempComp = mComponentList[componentName];
		GCLAssertMsg(tempComp, std::string("actor: ") + mName +"does not have the component: "+ componentName );
		return tempComp;
	}
	bool HasComponent(const std::string &componentName) const
	{
		return mComponentList.find(componentName)!= mComponentList.end();
	}

	void SendEvent(size_t event, void *arg)
	{
		for (auto it = mComponentList.begin(); it != mComponentList.end(); ++it)
		{
			Component *tempComponent = it->second;
			tempComponent->ProcessEvent(event, arg);
		}
	}

	void CreateComponent(const std::string &componentName);
	void SetPosition(Real x, Real y,Real z)
	{ SetPosition(WorldPoint3(x,y,z));	}
	void SetPosition(const WorldPoint3 &position);
private:
	typedef std::map<std::string, Component*> ComponentMap;
	ComponentMap mComponentList;
};

}
