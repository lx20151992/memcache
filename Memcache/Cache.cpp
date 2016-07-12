//
//  Cache.cpp
//  Memcache
//
//  Created by Ying Wang on 10/07/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#include "Cache.hpp"

namespace Memcache {

Cache* Cache::instance = 0;

Cache* Cache::Instance()
{
    if (instance == NULL)
    {
        instance = new Cache();
    }
    return instance;
}

bool Cache::HandleRequest(Request* request, Response*& response)
{
    if (request->Command() == COMMAND::GET)
    {
        std::cout << "GET " << request->KeyString() << " - " << cacheHt[request->KeyString()] << std::endl;
        response = new Response(request->Command(), request->ExtraString(), cacheHt[request->KeyString()]);
    }
    else if (request->Command() == COMMAND::SET)
    {
        std::cout << "SET " << request->KeyString() << " - " << request->ValueString() << std::endl;
        cacheMutex.lock();
        cacheHt[request->KeyString()] = request->ValueString();
        cacheMutex.unlock();
        response = new Response(request->Command(), request->ExtraString(), cacheHt[request->KeyString()]);
    }
    return true;
}

Cache::~Cache()
{
    delete instance;
}

}