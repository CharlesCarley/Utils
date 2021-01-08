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
#ifndef _skStreams_h_
#define _skStreams_h_

#include "skAllocator.h"

class skStream
{
public:
    enum Mode
    {
        UNDEFINED = -1,
        READ,       //!< Opens for reading binary data
        READ_TEXT,  //!< Opens for reading in text mode
        WRITE,      //!< Opens for writing binary data
        WRITE_TEXT  //!< Open in text mode
    };

public:
    skStream();
    virtual ~skStream();

    virtual void open(const char* path, int mode) = 0;
    virtual void close(void)                      = 0;
    virtual bool isOpen(void) const               = 0;
    virtual bool eof(void) const                  = 0;

    // return SK_NPOS on error or the number of bytes read
    virtual SKsize read(void* dst, SKsize nr) const = 0;

    // return SK_NPOS on error or the number of bytes written
    virtual SKsize write(const void* src, SKsize nr) = 0;

    virtual bool seek(SKint64 offset, SKsize dir) = 0;

    // returns the current position indicator
    virtual SKsize position(void) const = 0;

    // returns the size of the file.
    // Note that, some streams may not know the total size up front.
    virtual SKsize size(void) const = 0;

    SKsize writef(const char* format, ...);

    virtual void reserve(SKsize nr)
    {
        // not all streams need to preallocate memory
    }

    bool canRead() const
    {
        return m_mode == READ || m_mode == READ_TEXT;
    }

    bool canWrite() const
    {
        return m_mode == WRITE || m_mode == WRITE_TEXT;
    }

protected:
    SKint32 m_mode;
};

#endif  //_skStreams_h_
