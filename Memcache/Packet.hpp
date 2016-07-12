//
//  Packet.hpp
//  Memcache
//
//  Created by Ying Wang on 06/07/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#ifndef Packet_hpp
#define Packet_hpp

#include <iostream>
#include <arpa/inet.h>
#include <vector>

namespace Memcache {

typedef std::vector<uint8_t> PayloadData;
static const int HEADER_LENGTH = 24;

enum COMMAND
{
    GET,
    SET
};

struct Header
{
    uint8_t magic;
    uint8_t opcode;
    uint16_t keyLength;
    uint8_t extrasLength;
    uint8_t dataType;
    uint16_t status;
    uint32_t totalBodyLength;
    uint32_t opaque;
    uint64_t cas;
};

struct Payload
{
    PayloadData specificExtras;
    PayloadData key;
    PayloadData value;
};

class Packet
{
protected:
    Header* header;
    Payload* payload;
    COMMAND command;
    std::string keyString = "";
    std::string valueString = "";
    std::string extraString = "";
    
public:
    COMMAND Command() const { return command; }
    std::string KeyString() const { return keyString; }
    std::string ValueString() const { return valueString; }
    std::string ExtraString() const { return extraString; }
};

class Request : public Packet
{
public:
    Request(unsigned char data[], int len);
};

class Response : public Packet
{
private:
    unsigned char* buffer;
    int bufferSize;
public:
    Response(COMMAND opcode, std::string extra, std::string value);
    unsigned char* Buffer() const { return buffer;} ;
    void WriteBuffer();
    int BufferSize() const { return bufferSize; };
    ~Response();
};

}

#endif /* Packet_hpp */
