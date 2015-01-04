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
#if	defined( OS_ANDROID)
#include <sstream>
#include <gcl/Log.h>
#include <gcl/Exception.h>
#include <gcl/File.h>
using namespace GCL;
extern int main(int argc, char **argv);
#include <native_app_glue/android_native_app_glue.h>

#pragma GCC diagnostic ignored "-pedantic"
namespace GCL
{
	android_app* gAndroidApp = nullptr;
}
JNIEXPORT void android_main(struct android_app* state)
{
   	app_dummy();
   	gAndroidApp = state;
   	KLog("internal storage path: %s", gAndroidApp->activity->internalDataPath);
   	ResourceFile::RegisterAssetManager(gAndroidApp->activity->assetManager);
   	GCLFile::SetDataPath(gAndroidApp->activity->internalDataPath);
	try
	{
    char **argv=nullptr;
   	main(0, argv);
	}
	catch (GCLException & e)
	{
		std::stringstream str;
		str << "[FAILED] " << std::endl;
		str << e.what();
		KLog("%s", str.str().c_str());
	}
	catch (std::exception &e)
	{
		std::stringstream s;
		s << "[FAILED] " << e.what() << std::endl;
		KLog("%s", s.str().c_str());
	}
	catch (...)
	{
		KLog("[FAILED] not sure what happened");
	}
}
#endif
