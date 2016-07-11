//
//  Server.hpp
//  Memcache
//
//  Created by Ying Wang on 09/07/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include <iostream>
#include "Packet.hpp"
#include "Cache.hpp"
#include "ThreadPool.h"

class Server
{
private:
    void handleRequest(unsigned char bufIn[], int bytesIn, unsigned char bufOut[], int& bytesOut, int i);
    Cache* cache;
    ThreadPool* tPool;
    static const int NUM_THREADS = 10;

public:
    void init();
    int mainLoop();
    void stop();
};

#endif /* Server_hpp */
