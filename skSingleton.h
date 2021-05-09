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
#ifndef _skSingleton_h_
#define _skSingleton_h_

#include "Config/skConfig.h"

template <typename T>
class skSingleton
{
protected:
    static skSingleton* m_singleton;

public:
    skSingleton()
    {
        SK_ASSERT(!m_singleton);
        m_singleton = this;
    }

    virtual ~skSingleton()
    {
        SK_ASSERT(m_singleton);
        m_singleton = nullptr;
    }
};

#define SK_DECLARE_SINGLETON(cls)   \
    static cls& getSingleton(void); \
    static cls* getSingletonPtr(void);

#define SK_IMPLEMENT_SINGLETON(cls)                      \
    template <>                                          \
    skSingleton<cls>* skSingleton<cls>::m_singleton = 0; \
    cls&              cls::getSingleton(void)            \
    {                                                    \
        SK_ASSERT(m_singleton);                          \
        return *static_cast<cls*>(m_singleton);          \
    }                                                    \
    cls* cls::getSingletonPtr(void)                      \
    {                                                    \
        return static_cast<cls*>(m_singleton);           \
    }

#endif  //_skSingleton_h_
