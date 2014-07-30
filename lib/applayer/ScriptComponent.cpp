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

#include "applayer/ScriptComponent.h"

#include "applayer/Actor.h"
#include <script/ScriptResourceManager.h>

#include <sstream>

using namespace GCL;


GCL::ScriptComponent::ScriptComponent(Actor *parentActor, PtrLuaTableIterator &it)
	: Component(parentActor)
{
	mScriptResource = nullptr;
	if (!it)
		return;
	while (!it->End())
	{
		if (it->GetKey() == "filename")
		{
			SetScript(it->GetString());
		}
		++(*it);
	}
}


void ScriptComponent::SetScript(const std::string &filename)
{
	ScriptResourceManager &scriptManager = ScriptResourceManager::Instance();
	if (mScriptResource)
		scriptManager.ReleaseResource(mScriptResource);
	std::stringstream s;
	s<<SCRIPT_PATH<<filename<<".luac";
	const Resource *resource = scriptManager.LoadResource(s.str().c_str());
	const ScriptResource *luaResource = static_cast<const ScriptResource*>(resource);
	mScriptResource = luaResource;

	mScriptResource->ExecuteFunction("Initialize", mParentActor);
}
ScriptComponent::~ScriptComponent()
{
	mScriptResource->ExecuteFunction("Terminate", mParentActor);
	ScriptResourceManager::Instance().ReleaseResource(mScriptResource);
}

void GCL::ScriptComponent::ProcessEvent( size_t , void * )
{

}

void ScriptComponent::Update(Real )
{
	mScriptResource->ExecuteFunction("Logic", mParentActor);
}

void GCL::ScriptComponent::Render(const Matrix44 &)
{
	//sprite is auto registered by application, no need
}

void GCL::ScriptComponent::PostInit()
{
}
