#include <iostream>

#include "GCLApplicationTest.h"
#include "GCLApplicationCameraTest.h"
#include "GCLRenderObjectTest.h"
int main(int /*argc*/, char ** /*argv*/)
{
	try
	{
		GCLApplicationTest::Test();
		GCLApplicationCameraTest::Test();
		GCLRenderObjectTest::Test();
	}
	catch (GCLException &e)
	{
		std::stringstream str;
		str << "[FAILED] \n";
		str << e.what();

		str << std::endl;
		std::cerr << str.str();
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
