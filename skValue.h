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
#ifndef _skValue_h_
#define _skValue_h_

#include <typeinfo>
#include "skAllocator.h"

#if SK_PLATFORM != SK_PLATFORM_ANDROID

class skValue
{
public:
    typedef std::type_info Info;

protected:
    class Value
    {
    public:
        virtual ~Value() = default;

        virtual const Info& type(void) const  = 0;
        virtual Value*      clone(void) const = 0;
    };

    template <typename T>
    class ValueType : public Value
    {
    public:
        ValueType(const T& v) :
            m_value(v)
        {
        }

        virtual ~ValueType()
        {
        }

        SK_INLINE const Info& type(void) const override
        {
            return typeid(T);
        }

        SK_INLINE Value* clone(void) const override
        {
            return new ValueType<T>(m_value);
        }

        T m_value;
    };

    Value* m_data;

    SK_INLINE skValue& swap(skValue& rhs) noexcept
    {
        skSwap(m_data, rhs.m_data);
        return *this;
    }

public:
    skValue() :
        m_data(nullptr)
    {
    }

    skValue(const skValue& v) :
        m_data(v.m_data ? v.m_data->clone() : nullptr)
    {
    }

    template <typename T>
    skValue(const T& v) :
        m_data(new ValueType<T>(v))
    {
    }

    ~skValue()
    {
        delete m_data;
    }

    template <typename T>
    SK_INLINE skValue& operator=(const T& rhs)
    {
        skValue(rhs).swap(*this);
        return *this;
    }

    SK_INLINE skValue& operator=(const skValue& rhs)
    {
        skValue(rhs).swap(*this);
        return *this;
    }

    template <typename T>
    SK_INLINE operator T(void) const
    {
        if (m_data && m_data->type() == typeid(T))
            return static_cast<ValueType<T>*>(m_data)->m_value;
        return T();
    }

    template <typename T>
    SK_INLINE T get(const T& def = T()) const
    {
        if (m_data && m_data->type() == typeid(T))
            return static_cast<ValueType<T>*>(m_data)->m_value;
        return def;
    }

    SK_INLINE bool isTypeOf(const skValue& v) const
    {
        return m_data && v.m_data && m_data->type() == v.m_data->type();
    }
    SK_INLINE bool isTypeOf(const Info& v) const
    {
        return m_data && m_data->type() == v;
    }
};
#endif
#endif  //_skValue_h_
