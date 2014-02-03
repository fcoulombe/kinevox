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

#pragma once
#include <gcl/Resource.h>

namespace GCL
{
	enum ShaderType
	{
		VERTEX_SHADER=0,
		FRAGMENT_SHADER=1,
		GEOMETRY_SHADER=2
	};
	class ShaderResource : public Resource
	{
	public:

		ShaderResource(const char *textureName);
		~ShaderResource();

		static const ShaderResource EmptyShader;
		const char *GetShaderSource() const { return mShaderSource; }
		const std::string &GetFilename() const { return mShaderFileName; }

		void SetShaderType(ShaderType type) const { mType = type; }
		ShaderType GetShaderType() const { return mType; }
	private:
		ShaderResource() {}
		char *mShaderSource;
		std::string mShaderFileName;
		mutable ShaderType mType;
	};
}
