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

#pragma once

#include <stdint.h>
#include <list>
#include <gcl/Assert.h>
#include <3rdparty/GLFW.h>
namespace GCL
{
    enum KeyNames
    {
        GCL_ESCAPE = GLFW_KEY_ESC,
        GCL_UP = GLFW_KEY_UP,
        GCL_DOWN = GLFW_KEY_DOWN,
        GCL_LEFT = GLFW_KEY_LEFT,
        GCL_RIGHT = GLFW_KEY_RIGHT,
        GCL_PAGEUP = GLFW_KEY_PAGEUP,
        GCL_PAGEDOWN = GLFW_KEY_PAGEDOWN,
    };

    class KeyListener;
    typedef std::list<KeyListener*> KeyListenerList;
    class EventManager
    {
    public:
        static void Initialize();
        static void Terminate();
        EventManager()
        {

        }
        ~EventManager()
        {
            mKeyListenerList.clear();
        }
     
        static EventManager &Instance()
        {
            GCLAssert(smpInstance);
            return *smpInstance;
        }
        
        void KeyDown(uint32_t key);
        void KeyUp(uint32_t key);
        void RegisterKeyListener(KeyListener *listener)
        {
            mKeyListenerList.push_back(listener);
        }
        void UnRegisterKeyListener(KeyListener *listener)
        {
            mKeyListenerList.remove(listener);
        }
    private:
        static EventManager *smpInstance;
        KeyListenerList mKeyListenerList;
    };

    class KeyListener 
    {
    public:
        KeyListener() 
        {
            EventManager::Instance().RegisterKeyListener(this);
        }
        virtual ~KeyListener() 
        {
            EventManager::Instance().UnRegisterKeyListener(this);
        }

        virtual void KeyDown(uint32_t key) = 0;

        virtual void KeyUp(uint32_t key) = 0;
    };
}
