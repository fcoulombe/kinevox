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

#include "applayer/SpriteComponent.h"

#include "applayer/Actor.h"
#include "applayer/Sprite.h"
#include <renderer/RenderObject.h>

using namespace GCL;

SpriteComponent::~SpriteComponent()
{
	delete mSprite;
}

GCL::SpriteComponent::SpriteComponent(Actor *parentActor, PtrLuaTableIterator &it)
	: Component(parentActor)
{
	while (!it->End())
	{
		if (it->GetKey() == "sprite")
		{
			const std::string spritePath =  it->GetString();
			mSprite = new Sprite(spritePath.c_str());
		}
		++(*it);
	}
}



void GCL::SpriteComponent::ProcessEvent( size_t , void * )
{

}

void SpriteComponent::Update(Real )
{
	mSprite->SetTransform(mParentActor->GetTransform());
	mSprite->Update();
}

void GCL::SpriteComponent::Render()
{
	//sprite is auto registered by application, no need
}
