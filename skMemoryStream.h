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
#ifndef _skMemoryStream_h_
#define _skMemoryStream_h_

#include "skStreams.h"

class skMemoryStream : public skStream
{
public:
    skMemoryStream(int mode = -1);
    virtual ~skMemoryStream();

    void open(const char* path, int mode);
    void open(const skStream& other, int mode);
    void open(const void* data, SKsize sizeInBytes, int mode);

    // Open the stream from an external data source.
    // The default behavior of this class is to clone the data
    // into the class's internal storage location of m_data. 
    // Using this method will allow 'm_data' to be stored elsewhere,
    // but provide the same functionality.
    void open(const void* data, SKsize sizeInBytes, SKsize posInData, bool externalData);

    void   close(void);
    SKsize read(void* dst, SKsize nr) const;
    SKsize write(const void* src, SKsize nr);
    void   clear(void);
    bool   seek(SKint64 offset, SKsize dir);
    void   reserve(SKsize nr);

    inline SKsize position(void) const
    {
        return m_pos;
    }

    inline SKsize size(void) const
    {
        return m_size;
    }

    inline bool isOpen(void) const
    {
        return m_data != 0;
    }

    inline bool eof(void) const
    {
        return m_pos == SK_NPOS || m_pos >= m_size;
    }

    inline SKbyte* ptr()
    {
        return m_data;
    }

    inline const SKbyte* ptr() const
    {
        return m_data;
    }

    SKbyte* addressAtPosition() const
    {
        if (m_pos < m_size && m_data && m_size > 0)
            return &m_data[m_pos];
        return nullptr;
    }

    void   readInt8(SKuint8& out) const;
    void   readInt16(SKuint16& out) const;
    void   readInt32(SKuint32& out) const;
    void   readInt64(SKuint64& out) const;
    void   readVaryingInt(SKsize& out) const;
    SKsize getVaryingInt() const;

    // Reads up to the size of the destination buffer (destLen)
    // or a null terminating character.
    void readString(SKbyte* dest, SKuint32 destLen) const;

    // This is for use with null terminated string arrays.
    // Seeks the current position to the next location
    // after the null terminator, or to the end of the stream,
    // which ever comes first.
    // returns zero if an error occurs, and the total number
    // of bytes scanned on success.
    // Access to the current string can be obtained by 
    // calling: 
    //      char *string1 = stream.addressAtPosition();
    //      stream.seekString();
    //      char *string2 = stream.addressAtPosition();
    //      
    SKsize seekString() const;

private:
    SKbyte*        m_data;
    mutable SKsize m_pos;
    SKsize         m_size, m_capacity, m_initialOffset;
    bool           m_isExternal;

    bool isValidStream(int offs) const;
};

#endif  //_skMemoryStream_h_
