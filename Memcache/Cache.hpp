//
//  Cache.hpp
//  Memcache
//
//  Created by Ying Wang on 10/07/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#ifndef Cache_hpp
#define Cache_hpp

#include <iostream>
#include <unordered_map>
#include <string>
#include <mutex>
#include "Packet.hpp"

typedef std::unordered_map<std::string, std::string> CacheHT;

class Cache
{
private:
    CacheHT cacheHt;
    static Cache *instance;
    std::mutex cacheMutex;
    
public:
    static Cache* Instance();
    bool HandleRequest(Request* in, Response*& out);
    ~Cache();
};

#endif /* Cache_hpp */
