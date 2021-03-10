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
#ifndef _skUserObject_h_
#define _skUserObject_h_

#include "Config/skConfig.h"

class skUserObject
{
private:
    SKuintPtr m_object;

public:
    skUserObject() :
        m_object(0)
    {
    }

    ~skUserObject()
    {
        SK_ASSERT(!m_object && "Object not unbound");
    }

    void bind(const SKuintPtr obj)
    {
        SK_ASSERT(obj && !m_object);
        m_object = obj;
    }

    void bind(void* obj)
    {
        bind(reinterpret_cast<SKuintPtr>(obj));
    }

    void unbind(const SKuintPtr obj)
    {
        if (obj == 0 || m_object == obj)
            m_object = 0;
    }

    void unbind(void* obj)
    {
        unbind(reinterpret_cast<SKuintPtr>(obj));
    }

    template <typename T>
    T* cast(void)
    {
        SK_ASSERT(m_object);
        return reinterpret_cast<T*>(m_object);
    }

    template <typename T>
    const T* cast(void) const
    {
        SK_ASSERT(m_object);
        return reinterpret_cast<const T*>(m_object);
    }

    SKuintPtr get(void) const
    {
        return m_object;
    }

    bool bound(void) const
    {
        return m_object != 0;
    }
};

#endif  //_skUserObject_h_
