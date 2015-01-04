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

#include "applayer/Actor.h"
#include <algorithm>

#include "applayer/GCLApplication.h"
#include "applayer/RigidBodyComponent.h"

#include <gcl/BufferReader.h>
#include <gcl/BufferWriter.h>

#include <script/ConfigLua.h>

using namespace GCL;

void Actor::CreateComponent(const std::string &componentName)
{
	PtrLuaTableIterator it(nullptr);
	std::pair<std::string, Component *> tempComponent = Component::CreateComponent(this, componentName,  it);
	mComponentList[tempComponent.first] = (tempComponent.second);
	Component *tempComponent2 = tempComponent.second;
	tempComponent2->PostInit();
}

Actor::Actor(BufferReader &buffer)
    : Node(buffer)
{
    buffer.Read(mAcrchetype);

    InternalInitialize();
    GCLApplication::RegisterRenderObject(this);
}

Actor::Actor(const char *name, const char *archetype)
	: Node(name),
    mAcrchetype(archetype)
{
    InternalInitialize();
	GCLApplication::RegisterRenderObject(this);
}

void Actor::InternalInitialize()
{
    std::string archFile(ARCHETYPE_PATH);
    archFile += mAcrchetype;
    archFile += ".arch";
    ConfigLua conf(archFile.c_str());
    PtrLuaTableIterator it = conf.GetTableIterator("Archetype.Components");
    while (!it->End())
    {
        const std::string componentName = it->GetKey();
        PtrLuaTableIterator compIt = it->GetTableIterator();
        std::pair<std::string, Component *> tempComponent = Component::CreateComponent(this, componentName, compIt );
        mComponentList.insert(tempComponent);
        (void)tempComponent;
        ++(*it);
    }
    //we need post init phase since some component depend on other but cannot 
    //guarantee that they will be created at first init time
    for (auto it = mComponentList.begin(); it != mComponentList.end(); ++it)
    {
        Component *tempComponent = it->second;
        tempComponent->PostInit();
    }

}

Actor::Actor(const char *name)
	: Node(name)
{
	GCLApplication::RegisterRenderObject(this);
}

Actor::~Actor()
{
	for (auto it = mComponentList.begin(); it != mComponentList.end(); ++it)
	{
		Component *tempComponent = it->second;
		delete tempComponent;
	}
	mComponentList.clear();
	
	GCLApplication::ReleaseRenderObject(this);
}


void Actor::SetPosition(const WorldPoint3 &position)
{
	auto it = mComponentList.find("RigidBodyComponent");
	if (it != mComponentList.end())
	{
		Component *tempComp = it->second;
		RigidBodyComponent *tempRigidBodyComp = static_cast<RigidBodyComponent*>(tempComp);
		tempRigidBodyComp->SetPosition(position);
	}
	Node::SetPosition(position);
}
#include <gcl/BufferWriter.h>
void GCL::Actor::SaveStates( BufferWriter &buffer )
{
    Node::SaveStates(buffer);

    buffer << mAcrchetype;
}


void GCL::Actor::PatchReference( const std::vector<ActorPtr> &actorList )
{
    uint32_t parentId = (uint32_t)size_t(mParentNode);
    if (parentId != (uint32_t)-1)
    {
        auto it = std::find_if(actorList.begin(), actorList.end(),
        [parentId](const ActorPtr actor) { return actor->GetId() ==  parentId; }); 
        mParentNode = it->get();
    }
    else
    {
        mParentNode = nullptr;
    }

    for (auto child = mChilds.begin(); child != mChilds.end(); ++child)
    {
        uint32_t childId = (uint32_t)size_t(*child);
        auto it = std::find_if(actorList.begin(), actorList.end(),
                [childId](const ActorPtr actor) { return actor->GetId() ==  childId; }); 
        *child = it->get();
    }
}
