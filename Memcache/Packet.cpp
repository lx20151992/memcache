//
//  Packet.cpp
//  Memcache
//
//  Created by Ying Wang on 06/07/16.
//  Copyright © 2016 Ying. All rights reserved.
//

#include "Packet.hpp"

namespace Memcache {

void Response::WriteBuffer()
{
    bufferSize = HEADER_LENGTH + header->totalBodyLength;
    buffer = new unsigned char[bufferSize];
    memset(buffer, 0, bufferSize * sizeof(unsigned char));
    buffer[0] = header->magic;
    buffer[1] = header->opcode;
    buffer[4] = header->extrasLength;
  
    int tbl = header->totalBodyLength;
    for (int i = 11; i >= 8; i--)
    {
        buffer[i] = tbl % 256;
        tbl /= 256;
    }
  
    if (header->opcode == COMMAND::GET)
    {
        std::copy(payload->value.begin(), payload->value.end(), buffer + HEADER_LENGTH + header->extrasLength + header->keyLength);
    }

    std::cout << std::dec << bufferSize << std::endl;
    for (int i = 0; i < bufferSize; i++)
    {
        if (i < 28)
            std::cout << std::hex << unsigned(buffer[i]) << " ";
        else
            std::cout << (buffer[i]);
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

Response::Response(COMMAND opcode, std::string extra, std::string value)
{
    header = new Header();
    payload = new Payload();
    header->magic = 0x81;
    if (opcode == COMMAND::GET)
    {
        header->opcode = 0x00;
        header->extrasLength = 4;
        header->keyLength = 0;
        header->totalBodyLength = static_cast<int>(header->extrasLength + header->keyLength + value.size());
        std::copy(value.begin(), value.end(), std::back_inserter(payload->value));
    }
    else if (opcode == COMMAND::SET)
    {
        header->opcode = 0x01;
        header->extrasLength = 0;
        header->keyLength = 0;
        header->totalBodyLength = 0;
    }
    WriteBuffer();
}

Response::~Response()
{
    delete buffer;
    buffer = NULL;
}

Request::Request(unsigned char data[], int len)
{
    if (len < HEADER_LENGTH) return;
    header = reinterpret_cast<Header*>(data);
    header->keyLength = data[2] * 256 + data[3];
    payload = new Payload();
    // GET Request
    if (header->opcode == COMMAND::GET)
    {
        command = COMMAND::GET;
        for (int i = HEADER_LENGTH; i < HEADER_LENGTH + header->keyLength; i++)
        {
            payload->key.push_back(data[i]);
        }
        keyString = std::string(payload->key.begin(), payload->key.end());
    }
    // SET Request
    else if (header->opcode == COMMAND::SET)
    {
        command = COMMAND::SET;
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