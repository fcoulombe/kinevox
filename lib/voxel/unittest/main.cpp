#include <iostream>
#include <sstream>

#include <SDL.h>

#include "Voxel/Cube.h"
#include "Voxel/Grid.h"
#include "Voxel/Voxel.h"

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
