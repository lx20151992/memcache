//
//  Packet.cpp
//  Memcache
//
//  Created by Ying Wang on 06/07/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#include "Packet.hpp"

namespace Memcache {

Response::Response(COMMAND opcode)
{
    retBufferSize = HEADER_LENGTH;
    retBuffer = new unsigned char[HEADER_LENGTH];
    memset(retBuffer, 0, HEADER_LENGTH);
    retBuffer[0] = 0x81;
    if (opcode == COMMAND::GET)
    {
        retBuffer[1] = 0x00;
    }
    else if (opcode == COMMAND::SET)
    {
        retBuffer[1] = 0x01;
    }
}

Response::Response(COMMAND opcode, std::string extra, std::string value)
{
    header = new Header();
    header->totalBodyLength = static_cast<int>(4 + value.size());
    retBufferSize = HEADER_LENGTH + header->totalBodyLength;
    retBuffer = new unsigned char[HEADER_LENGTH + header->totalBodyLength];
    memset(retBuffer, 0, HEADER_LENGTH + 4);
    retBuffer[0] = 0x81;
    if (opcode == COMMAND::GET)
    {
        retBuffer[1] = 0x00;
    }
    else if (opcode == COMMAND::SET)
    {
        retBuffer[1] = 0x01;
    }
    retBuffer[4] = 4; //extra.size();
    int vs = 4 + static_cast<int>(value.size());
    if (vs == 0)
    {
        retBuffer[7] = 0x01;
        return;
    }
    for (int i = 11; i >= 8; i--)
    {
        retBuffer[i] = vs % 256;
        vs /= 256;
    }
    const uint8_t* p = reinterpret_cast<const uint8_t*>(value.c_str());
    memcpy(retBuffer + HEADER_LENGTH + 4, p, value.size());
}

Response::~Response()
{
    delete retBuffer;
    retBuffer = NULL;
}

Request::Request(unsigned char data[], int len)
{
    if (len < HEADER_LENGTH) return;
    header = reinterpret_cast<Header*>(data);
    // GET Request
    if (header->opcode == 0x00)
    {
        command = COMMAND::GET;
        header->keyLength = data[2] * 256 + data[3];
        payload = new Payload();
        for (int i = HEADER_LENGTH; i < HEADER_LENGTH + header->keyLength; i++)
        {
            payload->key.push_back(data[i]);
        }
        keyString = std::string(payload->key.begin(), payload->key.end());
    }
    // SET Request
    else if (header->opcode == 0x01)
    {
        command = COMMAND::SET;
        header->keyLength = data[2] * 256 + data[3];
        payload = new Payload();
        for (int i = HEADER_LENGTH; i < (HEADER_LENGTH + header->extrasLength); i++)
        {
            payload->specificExtras.push_back(data[i]);
        }
        header->extrasLength = data[4];
        int pos = HEADER_LENGTH + header->extrasLength;
        for (int i = pos; i < (pos + header->keyLength); i++)
        {
            payload->key.push_back(data[i]);
        }
        keyString = std::string(payload->key.begin(), payload->key.end());
        pos = pos + header->keyLength;
        header->totalBodyLength = data[8] * 256 * 256 * 256 + data[9] * 256 * 256 + data[10] * 256 + data[11];
        for (int i = pos; i < (HEADER_LENGTH + header->totalBodyLength); i++)
        {
            payload->value.push_back(data[i]);
        }
        valueString = std::string(payload->value.begin(), payload->value.end());
    }
}

}