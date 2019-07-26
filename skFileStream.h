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
#ifndef _skFileStream_h_
#define _skFileStream_h_

#include "skStreams.h"

class skFileStream : public skStream
{
public:
    typedef void* Handle;

public:
    skFileStream();
    virtual ~skFileStream();

    void   flush(void);
    void   open(const char* path, Mode mode);
    void   close(void);
    
    bool   eof(void) const;
    SKsize read(void* dst, SKsize nr) const;
    SKsize write(const void* src, SKsize nr);
    SKsize position(void) const;
    SKsize size(void) const;
    void   seek(SKsize offs, SKsize dir);


    SK_INLINE bool isOpen(void) const
    {
        return m_mode != NO_INPUT && m_handle != 0;
    }


private:
    Handle         m_handle;
    mutable SKsize m_pos;
    mutable SKsize m_size;
    SKint32        m_mode;
};

#endif  //_skFileStream_h_
