#include <iostream>

#include "GCLApplicationTest.h"
#include "GCLApplicationCameraTest.h"
#include "GCLRenderObjectTest.h"
#include "GCLRenderObjectWithMaterialTest.h"
#include "GCLRenderObjectMultipleRenderTest.h"
#include "GCLRenderTargetTest.h"
#include "SpriteTest.h"

int main(int /*argc*/, char ** /*argv*/)
{
	try
	{
		GCLApplicationTest::Test();
		GCLApplicationCameraTest::Test();
		GCLRenderObjectTest::Test();
		GCLRenderTargetTest::Test();
		GCLRenderObjectWithMaterialTest::Test();
		GCLRenderObjectMultipleRenderTest::Test();
		SpriteTest::Test();
	}
	catch (GCLException &e)
	{
		std::stringstream str;
		str << "[FAILED] \n";
		std::string what = e.what();
		str << what;

		str << std::endl;
		std::string finalOutput;
		finalOutput = str.str();
		std::cerr << finalOutput;
		return -1;
	}
	catch (std::exception &e)
	{
		std::cerr << "[FAILED] " << e.what() << std::endl;
		return -1;
	}
	catch (...)
	{
		std::cerr << "[FAILED] not sure what happened" << std::endl;
	}

	return 0;
}
