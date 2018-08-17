//
//  client.cpp
//  echoServer
//
//  Created by Alwin Mathew on 8/14/18.
//  Copyright © 2018 Alwin Mathew. All rights reserved.
//

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/FIFOBuffer.h>

#include <iostream>
#include <string>

using namespace Poco::Net;
using namespace Poco;
using namespace std;

#define BUFFER_SIZE 4096

int main(int argc, const char * argv[])
{
    if (argc < 2)
    {
        cout << "Error! Too few arguments.\nUsage: ./" << argv[0] << " [port]" << endl;
    }

    //connect via TCP to localhost address (0.0.0.0) with port input
    string addressString = std::string("0.0.0.0:") + argv[1];
    SocketAddress clientSocket(addressString);
    StreamSocket clientStream(clientSocket);
    
    //create FIFO buffer for TCP interprocess communication
    FIFOBuffer buffer(BUFFER_SIZE);
    char recievebuffer[BUFFER_SIZE];
    char sendbuffer[BUFFER_SIZE];
    
    //ask the user to input some data to send to echo server
    
    
    for (;;)
    {
        cout << "Please enter some data to send to the echo server. \nType 'exit' to exit client." << endl;
        cin.getline(sendbuffer, sizeof(sendbuffer));
        buffer.drain();
        buffer.copy(sendbuffer, BUFFER_SIZE);
        
        if (strcmp(sendbuffer, "exit") == 0)
        {
            cout << "Exiting..." << endl;
            clientStream.shutdown();
            clientSocket.~SocketAddress();
            buffer.drain();
            buffer.~BasicFIFOBuffer();
            exit(0);
        }
        
        clientStream.sendBytes(buffer);
        cout << "Sent:" << sendbuffer << endl;
        
        buffer.drain();
        
        clientStream.receiveBytes(buffer);
        buffer.read(recievebuffer, BUFFER_SIZE);
        cout << "Recieved:" << recievebuffer << endl;
        
    }
    
}
