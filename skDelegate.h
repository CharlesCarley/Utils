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
#ifndef _skDelegate_h_
#define _skDelegate_h_

#include "Config/skConfig.h"

template <typename R, typename A1>
class skSimpleDelegate
{
public:
    skSimpleDelegate() :
        m_object(nullptr),
        m_call(nullptr)
    {
    }

    skSimpleDelegate(const skSimpleDelegate& rhs) :
        m_object(rhs.m_object),
        m_call(rhs.m_call)
    {
    }

    template <typename T, R (T::*Method)(A1)>
    static skSimpleDelegate bind(T* obj)
    {
        skSimpleDelegate d;
        d.m_object = reinterpret_cast<void*>(obj);
        d.m_call   = &skSimpleDelegate::template call<T, Method>;
        return d;
    }

    SK_INLINE R operator()(A1 a)
    {
        return (*m_call)(m_object, a);
    }

    skSimpleDelegate& operator=(const skSimpleDelegate& rhs)
    {
        if (this != &rhs)
        {
            m_object = rhs.m_object;
            m_call   = rhs.m_call;
        }

        return *this;
    }

protected:
    typedef R (*CallbackType)(void* obj, A1);

    void*        m_object;
    CallbackType m_call;

    template <typename T, R (T::*Method)(A1)>
    static R call(void* obj, A1 a)
    {
        T* ptr = static_cast<T*>(obj);
        return (ptr->*Method)(a);
    }
};

template <typename R>
class skSimpleDelegate2
{
public:
    skSimpleDelegate2() :
        m_object(nullptr),
        m_call(nullptr)
    {
    }

    skSimpleDelegate2(const skSimpleDelegate2& rhs) :
        m_object(rhs.m_object),
        m_call(rhs.m_call)
    {
    }

    template <typename T, R (T::*Method)()>
    static skSimpleDelegate2 bind(T* obj)
    {
        skSimpleDelegate2 d;
        d.m_object = reinterpret_cast<void*>(obj);
        d.m_call   = &skSimpleDelegate2 ::template call<T, Method>;
        return d;
    }

    SK_INLINE bool isValid(void)
    {
        return (m_call != nullptr && m_object != nullptr);
    }

    SK_INLINE R operator()()
    {
        return m_call ? (*m_call)(m_object) : (R)0;
    }

    skSimpleDelegate2& operator=(const skSimpleDelegate2& rhs)
    {
        if (this != &rhs)
        {
            m_object = rhs.m_object;
            m_call   = rhs.m_call;
        }
        return *this;
    }

protected:
    typedef R (*CallbackType)(void* obj);

    void*        m_object;
    CallbackType m_call;

    template <typename T, R (T::*Method)()>
    static R call(void* obj)
    {
        return (static_cast<T*>(obj)->*Method)();
    }
};

#endif  //_skDelegate_h_
