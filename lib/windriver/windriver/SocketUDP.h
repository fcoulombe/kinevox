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

#include <stdint.h>

namespace GCL
{
    class Address
    {
    public:

        Address()
            : mAddress(0), mPort(0)
        {
        }
        Address( uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port )
        {
            mAddress = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
            mPort = port;
        }
        Address( uint32_t address, uint16_t port ) 
            : mAddress(address), mPort(port) {}
        uint32_t GetAddress() const { return mAddress; }
        uint8_t GetA() const{ return (uint8_t)((mAddress^0x00ffffff)>>24); }
        uint8_t GetB() const{ return (uint8_t)((mAddress^0xff00ffff)>>16); }
        uint8_t GetC() const { return (uint8_t)((mAddress^0xffff00ff)>>8); }
        uint8_t GetD() const { return (uint8_t)(mAddress^0xffffff00); }
        uint16_t GetPort() const { return mPort; }
        bool operator == ( const Address & other ) const { return mAddress == other.GetAddress() && mPort == other.GetPort(); }
        bool operator != ( const Address & other ) const { return mAddress != other.GetAddress() && mPort != other.GetPort(); }

    private:
        uint32_t mAddress;
        uint16_t mPort;
    };

    class SocketUDP
    {
    public:
        static void Initialize();
        static void Terminate();

        SocketUDP();
        ~SocketUDP();
        void Open( uint16_t port );
        void Close();
        bool IsOpen() const { return mHandle != -1; }
        bool Send( const Address & destination, const void * data, int size );
        int Receive( Address & sender, void * data, int size );

    private:
        int mHandle;
    };
}
