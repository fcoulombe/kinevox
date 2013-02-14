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
#include <windriver/SocketUDP.h>
#include <iostream>
#include <gcl/Assert.h>

#ifdef OS_WIN32
#include <winsock2.h>
#pragma comment( lib, "wsock32.lib" )
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#endif

using namespace GCL;

void GCL::SocketUDP::Initialize()
{
#ifdef OS_WIN32
    WSADATA WsaData;
    bool ret = WSAStartup( MAKEWORD(2,2), &WsaData ) == NO_ERROR;
    GCLAssert(ret);
#endif
}

void GCL::SocketUDP::Terminate()
{
#ifdef OS_WIN32
    WSACleanup();
#endif
}

GCL::SocketUDP::SocketUDP()
    : mHandle(-1)
{

}

GCL::SocketUDP::~SocketUDP()
{
    if (IsOpen())
        Close();
}

void GCL::SocketUDP::Open( uint16_t port )
{
    mHandle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    GCLAssertMsg( mHandle > 0 , "failed to create socket\n" );

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );

    int ret =  bind( mHandle, (const sockaddr*) &address, sizeof(sockaddr_in) );
    GCLAssertMsg(ret >= 0, "failed to bind socket\n" );
        
    //set non blocking since we don't want the program to stall on receive command
#if defined(OS_LINUX) || defined(OS_MACOSX)
    int nonBlocking = 1;
    ret = fcntl( mHandle, F_SETFL, O_NONBLOCK, nonBlocking );
    GCLAssertMsg(ret != -1,"failed to set non-blocking socket\n" );
#elif defined(OS_WIN32)
    DWORD nonBlocking = 1;
    ret = ioctlsocket( mHandle, FIONBIO, &nonBlocking );
    GCLAssertMsg(ret == 0 , "failed to set non-blocking socket\n" );
#else
#error "wrong platform"
#endif
}

void GCL::SocketUDP::Close()
{
#if defined(OS_MACOSX) || defined(OS_LINUX)
    close( mHandle );
#elif defined(OS_WIN32)
    closesocket( mHandle );
#else
#error "wrong platform"
#endif
}


bool GCL::SocketUDP::Send( const Address & destination, const void * data, int size )
{
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl( destination.GetAddress() );
    address.sin_port = htons( destination.GetPort() );

    int sent_bytes = sendto( mHandle, (const char*)data, size,
        0, (sockaddr*)&address, sizeof(sockaddr_in) );

    if ( sent_bytes != size )
    {
        std::cerr << "failed to send packet: return value = " << sent_bytes <<std::endl;
        return false;
    }
    return true;
}

int GCL::SocketUDP::Receive( Address & sender, void * data, int size )
{
        uint8_t packet_data[256];
        size_t maximum_packet_size = sizeof( packet_data );

#ifdef OS_WIN32
        typedef int socklen_t;
#endif

        sockaddr_in from;
        socklen_t fromLength = sizeof( from );

        int received_bytes = recvfrom( mHandle, (char*)data, size,
            0, (sockaddr*)&from, &fromLength );


        uint32_t from_address = ntohl( from.sin_addr.s_addr );
        uint16_t from_port = ntohs( from.sin_port );
        sender = Address(from_address, from_port);

        return received_bytes;
}
