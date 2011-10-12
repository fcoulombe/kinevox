#pragma once
#include <cstdlib>
#include <stdint.h>
#include <GCL/Rect.h>


namespace GCL
{
	const Rect<int> &ProcessSelection();
	void ProcessInput();
	bool IsKeyUp(uint16_t key);
	bool IsLMouseDown();
	size_t GetMouseX();
	size_t GetMouseY();
}
