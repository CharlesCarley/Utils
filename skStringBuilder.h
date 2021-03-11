/*
-------------------------------------------------------------------------------

    Copyright (c) Charles Carley.

    Contributor(s): none yet.

-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#ifndef __skStringBuilder_h__
#define __skStringBuilder_h__

#include "Utils/skString.h"

enum skStringBuilderStrategy
{
    ALLOC_N_BYTE_BLOCK,
    ALLOC_MUL2,
};

class skStringBuilder
{
private:
    SKbyte*  m_buffer;
    SKsize   m_size;
    SKsize   m_capacity;
    SKsize   m_nByteBlock;
    SKuint8  m_strategy;
    skString m_dest;

    void reserve(SKsize len);

    SKsize getNextCapacity(SKsize len) const;

    SKsize writeToBuffer(const void* source, SKsize len);

public:
    skStringBuilder() :
        m_buffer(nullptr),
        m_size(0),
        m_capacity(0),
        m_nByteBlock(256),
        m_strategy()
    {
        setStrategy(ALLOC_N_BYTE_BLOCK, m_nByteBlock);
    }

    explicit skStringBuilder(SKsize nBytes) :
        m_buffer(nullptr),
        m_size(0),
        m_capacity(0),
        m_nByteBlock(),
        m_strategy()
    {
        setStrategy(ALLOC_N_BYTE_BLOCK, nBytes);
    }

    skStringBuilder(SKuint8 strategy, SKsize nBytes) :
        m_buffer(nullptr),
        m_size(0),
        m_capacity(0),
        m_nByteBlock(),
        m_strategy()
    {
        setStrategy(strategy, nBytes);
    }

    ~skStringBuilder()
    {
        clear();
    }

    void setStrategy(SKuint8 strategy, SKsize nBytes)
    {
        m_strategy   = skClamp<SKuint8>(strategy, 0, 1);
        m_nByteBlock = skClamp<SKsize>(nBytes, 8, SK_MAX16);
        reserve(m_nByteBlock);
    }

    SKsize size() const
    {
        return m_size;
    }

    SKsize capacity() const
    {
        return m_capacity;
    }

    skString toString() const
    {
        // discouraged
        return skString(m_buffer, size());
    }

    void toString(skString& dest) const
    {
        dest.clear();
        dest.append(m_buffer, size());
    }

    void clear();

    void write(const skStringBuilder& oth);

    void write(const skString& str);

    void write(SKbyte ch);

    void write(const SKbyte* ptr);

    void write(const SKbyte* ptr, SKsize len);

    void write(SKint16 i16);

    void write(SKint32 i32);

    void write(SKint64 i64);

    void write(SKuint16 u16);

    void write(SKuint32 u32);

    void write(SKuint64 u64);

    void write(double d);

    void write(float f);
};

#endif  //__skStringBuilder_h__
