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

    void   flush(void);
    void   open(Mode mode);
    void   open(const char* path, Mode mode);
    void   open(const skStream& path, Mode mode);
    void   open(const void* data, SKsize sizeInBytes, Mode mode);
    void   close(void);
    SKsize read(void* dst, SKsize nr) const;
    SKsize write(const void* src, SKsize nr);
    void   clear(void);
    void   seek(SKint64 offset, SKsize dir);
    void   reserve(SKsize nr);

    SK_INLINE SKsize position(void) const
    {
        return m_pos;
    }

    SK_INLINE SKsize size(void) const
    {
        return m_size;
    }

    SK_INLINE bool isOpen(void) const
    {
        return m_buffer != 0;
    }

    SK_INLINE bool eof(void) const
    {
        return m_pos != npos;
    }


    SK_INLINE SKbyte* ptr()
    {
        return m_buffer;
    }

    SK_INLINE const SKbyte* ptr() const
    {
        return m_buffer;
    }

private:
    SKbyte* m_buffer;

    // This needs to be accessible in (read () const)
    // TODO: find code where const skMemoryStream& is calling read
    mutable SKsize m_pos;

    SKsize m_size, m_capacity;
    int    m_mode;
};

#endif  //_skMemoryStream_h_
