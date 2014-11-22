/*
* Copyright (C) 2013 by Francois Coulombe
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "windriver/EventHandler.h"

using namespace GCL;

EventManager *EventManager::smpInstance=NULL;

void EventManager::Initialize()
{
    GCLAssert(smpInstance == NULL);
    smpInstance = new EventManager();
}

void EventManager::Terminate()
{
    GCLAssert(smpInstance);
    delete smpInstance;
    smpInstance = NULL;
}

void GCL::EventManager::KeyDown( uint32_t key )
{
    for (auto it = mInputListenerList.begin(); it != mInputListenerList.end(); ++it)
    {
        InputListener *listener = *it;
        listener->KeyDown(key);
    }
}

void GCL::EventManager::KeyUp( uint32_t key )
{
    for (auto it = mInputListenerList.begin(); it != mInputListenerList.end(); ++it)
    {
        InputListener *listener = *it;
        listener->KeyUp(key);
    }
}
void GCL::EventManager::MouseDown( size_t x, size_t y )
{
    for (auto it = mInputListenerList.begin(); it != mInputListenerList.end(); ++it)
    {
        InputListener *listener = *it;
        listener->ClickDown(x,y);
    }
}

void GCL::EventManager::MouseUp( size_t x, size_t y)
{
    for (auto it = mInputListenerList.begin(); it != mInputListenerList.end(); ++it)
    {
        InputListener *listener = *it;
        listener->ClickUp(x,y);
    }
}
