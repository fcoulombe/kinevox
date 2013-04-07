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

#include <gcl/Assert.h>
#include <gcl/ResourceManager.h>
#include "script/ScriptResource.h"

struct lua_State;
namespace GCL
{
  struct ExposedLib;
  class LuaState;
  typedef int (*scriptFunction) (lua_State *L);
  class ScriptResourceManager : public ResourceManager
  {
  public:
    static void Initialize()
    {
      GCLAssert(smpInstance == NULL);
      smpInstance = new ScriptResourceManager();
    }
    static void Terminate()
    {
      GCLAssert(smpInstance != NULL);
      delete smpInstance;
      smpInstance = NULL;
    }
    static ScriptResourceManager &Instance() { GCLAssert(smpInstance != NULL);return *smpInstance;}

    Resource *Allocate(const char *filename);
    void Free(Resource * /*resource*/);

    void ExposeModule(const char *libName, scriptFunction constructor) const;
    void ExposeFunction(const char *funcName, scriptFunction func) const;
  private:
    static ScriptResourceManager *smpInstance;
    LuaState *mLuaState;
    LuaState &GetLuaState() { return *mLuaState; }
    ScriptResourceManager();
    ~ScriptResourceManager();
    friend class ScriptResource;
  };

}

