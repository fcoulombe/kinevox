#pragma once
#include <cstdlib>
#include <stdint.h>

struct CvRect;
namespace GCL
{
	const CvRect &ProcessSelection();
	void ProcessInput();
	bool IsKeyUp(uint16_t key);
	bool IsLMouseDown();
	size_t GetMouseX();
	size_t GetMouseY();
}
