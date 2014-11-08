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

#include "applayer/ScriptedGameState.h"
#include <sstream>
#include <script/ScriptResource.h>

using namespace GCL;


GCL::ScriptedGameState::ScriptedGameState( const std::string &name, const std::string &scriptFileName ) : GameState(name)
{
    ScriptResourceManager &scriptManager = ScriptResourceManager::Instance();
    std::stringstream s;
    s<<SCRIPT_PATH<<scriptFileName<<".luac";
    const Resource *resource = scriptManager.LoadResource(s.str().c_str());
    const ScriptResource *luaResource = static_cast<const ScriptResource*>(resource);
    mScriptResource = luaResource;

    mScriptResource->ExecuteMethod("Initialize", this);

}

bool GCL::ScriptedGameState::Update( Real dt )
{
    mScriptResource->ExecuteMethod("Logic", this, dt);
    return GameState::Update(dt);
}

GCL::ScriptedGameState::~ScriptedGameState()
{
    mScriptResource->ExecuteMethod("Terminate", this);
    ScriptResourceManager::Instance().ReleaseResource(mScriptResource);
}
