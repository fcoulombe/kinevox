/*
 * Copyright (C) 2011 by Francois Coulombe
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

namespace GCL
{
  //very simple resource manager for Mesh
  //when something asks it to load a Mesh, it will hash the name and then
  //look in the map if we already have this Mesh loaded. if its the case, it will return
  //a reference and increase the ref count. this is so that we don't waste memory loading similar Meshs
  class MeshResource;
  class MeshResourceManager : public ResourceManager
  {
  public:
    static void Initialize()
    {
      GCLAssert(smpInstance == NULL);
      smpInstance = new MeshResourceManager();
    }
    static void Terminate()
    {
      GCLAssert(smpInstance != NULL);
      delete smpInstance;
      smpInstance = NULL;
    }
    static MeshResourceManager &Instance() { GCLAssert(smpInstance != NULL);return *smpInstance;}

    Resource *Allocate(const char *filename);
    void Free(Resource * /*resource*/);

  private:
    static MeshResourceManager *smpInstance;

    MeshResourceManager() {}
  };

}

