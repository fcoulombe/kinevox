#include "Input/Input.h"
#include <SDL.h>
#include <cv.h>


namespace GCL
{



bool keys[350];
bool isLMousedown = false;
size_t mouseX=0;
size_t mouseY=0;

size_t GetMouseX()
{
	return mouseX;
}
size_t GetMouseY()
{
	return mouseY;
}
bool IsLMouseDown()
{
	return isLMousedown;
}
bool IsKeyUp(uint16_t key)
{

	return keys[key];

}
void ProcessInput()
{
	memset(keys, 0, 255);
	SDL_Event event;

	while ( SDL_PollEvent(&event) ) {
		switch (event.type) {
		case SDL_KEYDOWN:
			keys[event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			//printf("%d %s\n",event.key.keysym.sym, SDL_GetKeyName(event.key.keysym.sym));

			keys[event.key.keysym.sym] = false;

			break;
		case SDL_MOUSEMOTION:
			/*printf("Mouse moved by %d,%d to (%d,%d)\n",
						event.motion.xrel, event.motion.yrel,
						event.motion.x, event.motion.y);*/

			mouseX = event.button.x;
			mouseY = event.button.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			/*printf("Mouse button %d pressed at (%d,%d)\n",
						event.button.button, event.button.x, event.button.y);*/
			isLMousedown = true;

			mouseX = event.button.x;
			mouseY = event.button.y;
			break;
		case SDL_MOUSEBUTTONUP:
			/*printf("Mouse button %d pressed at (%d,%d)\n",
						event.button.button, event.button.x, event.button.y);*/
			isLMousedown = false;
			mouseX = event.button.x;
			mouseY = event.button.y;

			break;
		case SDL_QUIT:
			exit(0);
		}
	}
	if (mouseX>640) {
		mouseX -= 640;
	}

}


CvPoint origin;
CvPoint currentPos;
CvRect selection;
bool isSelectionStarted = false;
bool hasSelection = false;
const CvRect &ProcessSelection()
{
	currentPos.x = GetMouseX();
	currentPos.y = GetMouseY();
	if (IsLMouseDown()) {
		if (!isSelectionStarted) {
			//start picking
			origin = currentPos;
			selection.x = origin.x;
			selection.y = origin.y;
			selection.width = origin.x;
			selection.height = origin.y;

			printf("staret picking\n");
		}
		isSelectionStarted = true;
		selection.x = MIN(currentPos.x,origin.x);
		selection.y = MIN(currentPos.y,origin.y);
		selection.width = selection.x + CV_IABS(currentPos.x - origin.x);
		selection.height = selection.y + CV_IABS(currentPos.y - origin.y);

		selection.x = MAX( selection.x, 0 );
		selection.y = MAX( selection.y, 0 );
		selection.width = MIN( selection.width, 640);
		selection.height = MIN( selection.height, 480 );
		selection.width -= selection.x;
		selection.height -= selection.y;

	} else {
		if (isSelectionStarted) {
			printf("end picking\n");
			//end picking
			isSelectionStarted = false;
			hasSelection = true;
			/*selection.width = selection.x - currentPos.x;
			selection.height =selection.y -  currentPos.y;*/

		}
	}
	return selection;
}
}
