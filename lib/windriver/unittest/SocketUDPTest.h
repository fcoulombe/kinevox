/*
 * Copyright (C) 2011 by Francois Coulombe
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

#include <kinetestlib/UnitTest.h>
#include <gcl/Time.h>
#include <windriver/SocketUDP.h>

using namespace GCL;
namespace SocketUDPTest
{

void Test()
{
	TEST_START

    {
    Address add;
    Assert_Test(add.GetAddress() == 0);
    Assert_Test(add.GetPort() == 0);
    Assert_Test(add.GetA() == 0);
    Assert_Test(add.GetB() == 0);
    Assert_Test(add.GetC() == 0);
    Assert_Test(add.GetD() == 0);
    Address add2;
    Assert_Test(add == add2);
    Assert_Test(!(add != add2));
    }
    {
        Address add(192,168,1,101, 30000);
        Assert_Test(add.GetAddress() == 0xc0a80165);
        Assert_Test(add.GetPort() == 30000);
        Assert_Test(add.GetA() == 192);
        Assert_Test(add.GetB() == 168);
        Assert_Test(add.GetC() == 1);
        Assert_Test(add.GetD() == 101);
        Address add2(192,168,1,101, 30000);
        Assert_Test(add == add2);
        Assert_Test(!(add != add2));
    }

    {
        Address add(0xc0a80165, 30000);
        Assert_Test(add.GetAddress() == 0xc0a80165);
        Assert_Test(add.GetPort() == 30000);
        Assert_Test(add.GetA() == 192);
        Assert_Test(add.GetB() == 168);
        Assert_Test(add.GetC() == 1);
        Assert_Test(add.GetD() == 101);
        Address add2(192,168,1,101, 30000);
        Assert_Test(add == add2);
        Assert_Test(!(add != add2));
    }

    {
        SocketUDP::Initialize();
        // create socket

        const int port = 30350;
        SocketUDP socket;
        socket.Open( port ) ;

        // send a packet

        const char data[] = "hello world!";
        bool sendSuccess = socket.Send( Address(127,0,0,1,port), data, sizeof( data ) );
        Assert_Test(sendSuccess);
        // receive packets
        Time::SleepMs(100);
        
        while ( true )
        {
            Address sender;
            unsigned char buffer[256];
            int bytes_read = socket.Receive( sender, buffer, sizeof( buffer ) );
            if (bytes_read == -1)
            {
                Time::SleepMs(100);
                continue;
            }
            if ( !bytes_read )
                break;    
            std::cout << bytes_read << "bytes read for buffer: " << buffer << "from: " << (int)(sender.GetA()) << "."
                <<(int)(sender.GetB()) << "."<<(int)(sender.GetC()) 
                << "."<<(int)(sender.GetD()) << ":"<<(int)(sender.GetPort()) << std::endl;
            break;
        }

        SocketUDP::Terminate();
    }
}
}
