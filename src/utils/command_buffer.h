#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//


#include <stdlib.h>

#define COMMAND_BUFFER_BUFFER_SIZE (32)
#define COMMAND_BUFFER_TOKEN_SIZE (8)

class __FlashStringHelper;

class CCommandBuffer
{
public:
    CCommandBuffer();
    void reset();
    void handleInputChar(char ch);
    bool startsWith(const char* buf) const;
    bool startsWith(const __FlashStringHelper* buf) const;
    
    bool endsWith(const char* buf) const;
    const char *getInternalBuffer() const;
    size_t length() const;

    const char *getNextToken(const char *separators);

private:
    size_t m_tail;
    size_t m_token_tail;
    char m_buffer[COMMAND_BUFFER_BUFFER_SIZE];
    char m_token[COMMAND_BUFFER_TOKEN_SIZE];
};
