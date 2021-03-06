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

#include <gcl/Point4.h>
namespace GCL
{
class Texture;
class GPUProgram;
class Shader;
class Material
{
public:
	Material(const char *filename="Default");

	~Material();
	void Bind() const;
	void LoadMaterial(const char *filename);

	void SetTexture(const char *texture);
	void SetTexture(Texture *texture);

    GPUProgram *GetShader() const { return mProgram; }
    Texture *GetTexture() const { return mTexture; }
private:
	Material(Material &);
	void operator=(Material &);
	Texture *mTexture;
	mutable GPUProgram *mProgram; //mutable because we need to set uniforms before rendering
	Shader *mVertexShader;
	Shader *mFragmentShader;
	WorldPoint4 mAmbient;
	WorldPoint4 mDiffuse;
	WorldPoint4 mSpecular;
	WorldPoint4 mEmissive;
	Real mShininess;
};

}
