//
//  Server.cpp
//  Memcache
//
//  Created by Ying Wang on 09/07/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "ThreadPool.h"
#include "Server.hpp"

static const char *PORT = "11211";  // the port users will be connecting to
static const int BACKLOG = 10;	 // how many pending connections queue will hold

void Server::init()
{
    tPool = new ThreadPool(NUM_THREADS);
}

void Server::stop()
{
    delete tPool;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void Server::handleRequest(unsigned char bufIn[], int bytesIn, unsigned char bufOut[], int& bytesOut, int i)
{
    Request* packetIn = new Request(bufIn, bytesIn);
    Response* packetOut;
    cache->Instance()->HandleRequest(packetIn, packetOut);
    bytesOut = packetOut->BufferSize();
    memcpy(bufOut, packetOut->Buffer(), bytesOut);
    if (send(i, bufOut, bytesOut, 0) == -1)
    {
        perror("send");
    }
    delete packetIn;
    delete packetOut;
}

int Server::mainLoop()
{
    init();
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number
    
    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;
    
    unsigned char inBuf[256];    // buffer for client data
    unsigned char outBuf[256];
    int nbytesIn;
    int nbytesOut;
    
    char remoteIP[INET6_ADDRSTRLEN];
    
    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    int i, rv;
    
    struct addrinfo hints, *ai, *p;
    
    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);
    
    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0)
    {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0)
        {
            continue;
        }
        
        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }
        
        break;
    }
    
    // if we got here, it means we didn't get bound
    if (p == NULL)
    {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }
    
    freeaddrinfo(ai); // all done with this
    
    // listen
    if (listen(listener, BACKLOG) == -1)
    {
        perror("listen");
        exit(3);
    }
    
    // add the listener to the master set
    FD_SET(listener, &master);
    
    // keep track of the biggest file descriptor
    fdmax = listener;
    
    // main loop
    for(;;)
    {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }
        
        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            { // we got one!!
                if (i == listener)
                {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                                   (struct sockaddr *)&remoteaddr,
                                   &addrlen);
                    
                    if (newfd == -1)
                    {
                        perror("accept");
                    }
                    else
                    {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax)
                        {    // keep track of the max
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                               "socket %d\n",
                               inet_ntop(remoteaddr.ss_family,
                                         get_in_addr((struct sockaddr*)&remoteaddr),
                                         remoteIP, INET6_ADDRSTRLEN),
                               newfd);
                    }
                }
                else
                {
                    // handle request from a client
                    if ((nbytesIn = recv(i, inBuf, sizeof inBuf, 0)) <= 0)
                    {
                        // got error or connection closed by client
                        if (nbytesIn == 0)
                        {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        }
                        else
                        {
                            perror("recv");
                        }
                        close(i);
                        FD_CLR(i, &master); // remove from master set
                    }
                    else
                    {
                        auto result = tPool->enqueue(&Server::handleRequest, this, inBuf, nbytesIn, outBuf, nbytesOut, i);
                    }
                }
            }
        }
    }
    
    return 0;
}