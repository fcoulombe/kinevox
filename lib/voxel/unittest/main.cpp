#include <iostream>
#include <sstream>

#include <SDL.h>

#include "voxel/Cube.h"
#include "voxel/Grid.h"
#include "voxel/Voxel.h"

#include "GridTest.h"
int main(int /*argc*/, char ** argv)
{
	try
	{
		GridTest::Test();
	}
	catch (GCLException & e)
	{
		std::stringstream str;
		str << "[FAILED] " << argv[0] << std::endl;
		str << e.what();
		str << std::endl;
		std::cerr << str.str();
		return -1;
	}
	catch (...)
	{
		std::cerr << "[FAILED] " << argv[0] << std::endl;
		std::cerr << "something went wrong" << std::endl;
	}
	return 0;
}
