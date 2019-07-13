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
    typedef void* Handle;

public:
    skMemoryStream();

    virtual ~skMemoryStream();
    void flush(void);
    void open(Mode mode);
    void open(const char* path, Mode mode);
    void open(const skStream& path, Mode mode);
    void open(const void* data, SKsize sizeInBytes, Mode mode);
    void close(void);

    SK_INLINE bool isOpen(void) const
    {
        return m_buffer != 0;
    }

    SK_INLINE bool eof(void) const
    {
        return m_pos != SK_NPOS;
    }

    SKsize read(void* dst, SKsize nr) const;
    SKsize write(const void* src, SKsize nr);
    SKsize writef(const char* fmt, ...);

    SK_INLINE SKsize position(void) const
    {
        return m_pos;
    }

    SK_INLINE SKsize size(void) const
    {
        return m_size;
    }

    void clear(void);

    SKsize seek(SKint32 off, SKint32 way);
    void   reserve(SKsize nr);

private:
    char*          m_buffer;
    mutable SKsize m_pos;
    SKsize         m_size, m_capacity;
    int            m_mode;
};

#endif //_skMemoryStream_h_
