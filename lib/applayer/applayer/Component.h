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
#include <map>
#include <string>
#include <gcl/Matrix44.h>
#include <gcl/WorldUnit.h>
#include <script/ConfigLua.h>

namespace GCL
{
	class Actor;
class Component
{
public:
	Component(Actor *parentActor)
	{
		mParentActor = parentActor;
	}

	virtual ~Component()
	{
	}
	//basically any component can perform render commands but 
	//calling an empty virtual function is cheaper than calling posting a message
	virtual void Render(const Matrix44 &) {} 
	virtual void Update(Real dt)=0;
	virtual void PostInit() = 0;

	virtual void ProcessEvent(size_t , void *) {}

	typedef std::pair<const char *, Component *> (*ComponentCreation)(Actor *, PtrLuaTableIterator &);
	
	static void Register(const std::string &compName, ComponentCreation createFunc);
	static std::pair<const char *, Component *> CreateComponent(Actor *parentActor, const std::string &componentName, PtrLuaTableIterator &it);
private:
	static std::map<std::string, ComponentCreation> mComponentCreationMap;
protected:
	Actor *mParentActor;

};


}
