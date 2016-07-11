//
//  Cache.cpp
//  Memcache
//
//  Created by Ying Wang on 10/07/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#include "Cache.hpp"

Cache* Cache::instance = 0;

Cache* Cache::Instance()
{
    if (instance == NULL)
    {
        instance = new Cache();
    }
    return instance;
}

bool Cache::HandleRequest(Request* in, Response*& out)
{
    if (in->Command() == COMMAND::GET)
    {
        std::cout << "GET" << std::endl;
        out = new Response(in->Command(), in->ExtraString(), cacheHt[in->KeyString()]);
    }
    else if (in->Command() == COMMAND::SET)
    {
        std::cout << "SET" << std::endl;
        cacheMutex.lock();
        cacheHt[in->KeyString()] = in->ValueString();
        cacheMutex.unlock();
        out = new Response(in->Command());
    }
    return true;
}

Cache::~Cache()
{
    delete instance;
}