#pragma once
#include <cstdlib>
#include <stdint.h>
#include <map>
#include <SDL.h>

#include <gcl/Rect.h>


namespace GCL
{
class Input
{
public:
	static const Rect<int> &ProcessSelection();
	static void ProcessInput();
	static bool IsKeyUp(uint32_t key);
	static bool IsLMouseDown();
	static size_t GetMouseX();
	static size_t GetMouseY();


private:
	static std::map<uint32_t, bool> keys;
	static bool isLMousedown ;
	static size_t mouseX;
	static size_t mouseY;
};
}
