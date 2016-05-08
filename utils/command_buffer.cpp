//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "command_buffer.h" 

#include <string.h>

#include <avr/pgmspace.h>

//--------------------------------------------------------------
CCommandBuffer::CCommandBuffer()
{
    reset();
}

//--------------------------------------------------------------
void CCommandBuffer::reset()
{
    m_tail = 0;
    m_buffer[0] = 0;

    m_token_tail = 0;
    m_token[0] = 0;
}

//--------------------------------------------------------------
bool CCommandBuffer::startsWith(const char* buf) const
{
    size_t len = strlen(buf);
    if(len > m_tail)
        return false;
    return strncmp(m_buffer, buf, len) == 0;
}

//--------------------------------------------------------------
bool CCommandBuffer::startsWith(const __FlashStringHelper* buf) const
{
    char tmpBuf[20];

    if( !buf )
        return false;

    int length = strlen_P((const char *) buf);
    if (length == 0)
        return false;

    if( length > sizeof(tmpBuf ) - 1 ) 
        length = sizeof(tmpBuf ) - 1;

    strncpy_P(tmpBuf, (const char *) buf, length);
    tmpBuf[length] = 0;
    return startsWith(tmpBuf);
}

//--------------------------------------------------------------
bool CCommandBuffer::endsWith(const char* buf) const
{
    size_t len = strlen(buf);
    if(len > m_tail)
        return false;
    return strncmp(&m_buffer[m_tail-len], buf, len) == 0;
}


//--------------------------------------------------------------
void CCommandBuffer::handleInputChar(char ch)
{
    m_buffer[m_tail] = ch;
    m_tail++;

    if( m_tail > COMMAND_BUFFER_BUFFER_SIZE )
    {
        // never happen
        m_tail = 0;
    }
    else if( m_tail == COMMAND_BUFFER_BUFFER_SIZE )
    {
        // buffer overflow
        memcpy(&m_buffer[0], &m_buffer[1], COMMAND_BUFFER_BUFFER_SIZE-1);
        m_tail--;
    }

    m_buffer[m_tail] = 0;
}

//--------------------------------------------------------------
const char *CCommandBuffer::getInternalBuffer() const
{
    return m_buffer;
}

//--------------------------------------------------------------
size_t CCommandBuffer::length() const
{
    return m_tail;
}

//--------------------------------------------------------------
const char *CCommandBuffer::getNextToken(const char *separators)
{

    size_t pos = m_token_tail;
    // skip non separators
    while(pos < m_tail)
    {
        char ch = m_buffer[pos];
        if( strchr(separators, ch) != NULL )
            break;

        pos++;
    }

    // no next token case
    if( pos == m_tail )
        return NULL;

    // skip separators
    while(pos < m_tail)
    {
        char ch = m_buffer[pos];
        if( strchr(separators, ch) == NULL )
            break;

        pos++;
    }

    // no next token case
    if( pos == m_tail )
        return NULL;

    // copy token to special buf
    size_t tokenPos = 0;
    while(pos < m_tail)
    {
        char ch = m_buffer[pos];
        m_token[tokenPos] = ch;

        if( strchr(separators, ch) != NULL )
            break;

        pos++;
        tokenPos++;

        if(tokenPos == COMMAND_BUFFER_TOKEN_SIZE)
        {
            tokenPos--;
            break;
        }

    }

    m_token_tail = pos;
    m_token[tokenPos] = 0;
    return m_token;

}
