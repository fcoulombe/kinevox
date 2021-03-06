/*
* Copyright (C) 2014 by Francois Coulombe
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

#include <applayer/GCLApplication.h>
#include <applayer/Actor.h>
#include <applayer/GCLWorld.h>
#include <gcl/Exception.h>
#include <gcl/Math.h>
#include <gcl/Time.h>
#include <input/Input.h>
#include <renderer/Camera.h>


using namespace GCL;


int main(int /*argc*/, char ** /*argv*/)
{
    std::cout << "start program" << std::endl;

    try
    {
        GCLApplication::Initialize("World Example");
        GCLWorld world;
        Node &root = world.GetRootNode();
        Node *area = new Node("Area1");
        root.AddChildNode(*area);
        for (size_t j=0; j<5; ++j)
        {
            for (size_t i=0; i<10; ++i)
            {
                std::stringstream name;
                name << "floor";
                name << j;
                name << "x";
                name << i;
                Actor *tile = new Actor(name.str().c_str(), "RigidFloor");

                tile->SetPosition(10.0*j-25.0,-5.0,i*-10.0);
                area->AddChildNode(*tile);
            }
        }
        Actor *mario=nullptr;
        for (size_t i=0; i<10; ++i) {
            std::stringstream name;
            name << "Mario" << i;
            mario = new Actor(name.str().c_str(), "RigidBodyMario");
            area->AddChildNode(*mario);

            mario->SetOrientation(0.0, DegreeToRadian(180.0), 0.0);
            Real x = Randf(-5.0, 5.0);
            Real y = Randf(5.0, 10.0);
            Real z = Randf(-15., -10.);
            mario->SetPosition(x,y,z);
        }

        Camera cam;

        bool isRunning = true;
        const size_t TICKS_PER_SECOND = 60;
        const size_t SKIP_TICKS = 1000 / TICKS_PER_SECOND;
        const int MAX_FRAMESKIP = 5;
        size_t next_game_tick = GCL::Time::GetTickMs()-1;
        int loops;

        size_t lastTime = next_game_tick;
        while(isRunning)
        {
            loops = 0;
            size_t currentTime;
            while(GCL::Time::GetTickMs() > next_game_tick && loops < MAX_FRAMESKIP)
            {
                currentTime = GCL::Time::GetTickMs();
                Real deltaTime = (currentTime - lastTime) / 1000.0;
                lastTime = currentTime;
                area->Update(deltaTime);
                GCLApplication::SetViewportCamera(cam);
                GCLApplication::Update();
                if (Input::IsKeyUp(GCL_ESCAPE))
                    isRunning=false;
                const Real kSpeed = 0.1;
                if (Input::IsKeyUp(GCL_LEFT))
                {
                    WorldPoint3 position = mario->GetPosition();
                    position.x -= kSpeed;
                    mario->SetPosition(position);
                }
                if (Input::IsKeyUp(GCL_RIGHT))
                {
                    WorldPoint3 position = mario->GetPosition();
                    position.x += kSpeed;
                    mario->SetPosition(position);
                }
                if (Input::IsKeyUp(GCL_UP))
                {
                    WorldPoint3 position = mario->GetPosition();
                    position.z -= kSpeed;
                    mario->SetPosition(position);
                }
                if (Input::IsKeyUp(GCL_DOWN))
                {
                    WorldPoint3 position = mario->GetPosition();
                    position.z += kSpeed;
                    mario->SetPosition(position);
                }
                if (Input::IsKeyUp(GCL_PG_UP))
                {
                    WorldPoint3 position = mario->GetPosition();
                    position.y += kSpeed;
                    mario->SetPosition(position);
                }
                GCLApplication::Render();
                std::cout.flush();
                next_game_tick += SKIP_TICKS;
                loops++;
            }
        }
    }
    catch(GCLException &e)
    {
        std::cerr << e.what() << std::endl;
    }


    GCLApplication::Terminate();

    return 0;
}

