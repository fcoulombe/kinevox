#include <iostream>
#include <sstream>

#include <GCL/Exception.h>

#include "InputTest.h"

int main(int /*argc*/, char ** argv)
{
	try
	{
		InputTest::Test();
	}
	catch (GCLException &e)
	{
		std::stringstream str;
		str << "[FAILED] " << argv[0] << std::endl;
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
