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

#include "applayer/ScriptApi.h"
#include <string>
#include <script/ScriptResourceManager.h>
#include <script/Lua.h>
#include <sound/SoundManager.h>

using namespace GCL;

static int KPlaySound(lua_State * L)
{
	const char *soundFile = lua_tostring(L, 1);
    std::string fullPath(SOUND_PATH);
    fullPath += soundFile;
    auto sound = SoundManager::LoadSound(fullPath.c_str());
    sound->Play();
	return 1;
}

static int KPlayMusic(lua_State * L)
{
    const char *soundFile = lua_tostring(L, 1);
    std::string fullPath(MUSIC_PATH);
    fullPath += soundFile;
    auto sound = SoundManager::LoadMusic(fullPath.c_str());
    sound->Play();
    return 1;
}

static const luaL_Reg soundExposedFunc[] = {
	{ "PlaySound", KPlaySound},
    { "PlayMusic", KPlayMusic},
	{ NULL, NULL } };

int luaopen_soundLib(lua_State * L)
{
	luaL_newlib(L, soundExposedFunc);
	return 1;
}

