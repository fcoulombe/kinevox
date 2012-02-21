#include "renderer/ViewPort.h"


using namespace GCL;

#ifdef OS_IPHONE
    const size_t ViewPort::DEFAULT_SCREEN_WIDTH = 320;
    const size_t ViewPort::DEFAULT_SCREEN_HEIGHT = 480;
#else
    const size_t ViewPort::DEFAULT_SCREEN_WIDTH = 800;
    const size_t ViewPort::DEFAULT_SCREEN_HEIGHT = 600;
#endif

