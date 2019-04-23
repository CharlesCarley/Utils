/*
-------------------------------------------------------------------------------

    Copyright (c) Charles Carley.

    All Rights Reserved.

-------------------------------------------------------------------------------
*/
#ifndef _skValue_h_
#define _skValue_h_

#include "skAllocator.h"
#include <typeinfo>


#if SK_PLATFORM != SK_PLATFORM_ANDROID

class skValue : public skAllocObject
{
public:
    typedef std::type_info Info;


protected:

    class Value : public skAllocObject
    {
    public:
        virtual ~Value() {}
        virtual const Info&    type(void) const = 0;
        virtual Value*         clone(void) const = 0;
    };


    template <typename T>
    class ValueType : public Value
    {
    public:

        ValueType(const T& v)
            :   m_value(v)
        {
        }

        virtual ~ValueType() {}

        SK_INLINE const Info&   type(void) const
        {
            return typeid(T);
        }
        SK_INLINE Value*        clone(void) const
        {
            return new ValueType<T>(m_value);
        }

        T m_value;
    };

    Value* m_data;


    SK_INLINE skValue& swap(skValue& rhs)
    {
        skSwap(m_data, rhs.m_data);
        return *this;
    }

public:

    skValue()
        :   m_data(0)
    {
    }

    skValue(const skValue& v)
        :   m_data(v.m_data ? v.m_data->clone() : 0)
    {
    }

    template<typename T>
    skValue(const T& v)
        :   m_data(new ValueType<T>(v))
    {
    }

    ~skValue()
    {
        delete m_data;
    }

    template<typename T>
    SK_INLINE skValue& operator = (const T& rhs)
    {
        skValue(rhs).swap(*this);
        return *this;
    }

    SK_INLINE skValue& operator = (const skValue& rhs)
    {
        skValue(rhs).swap(*this);
        return *this;
    }


    template<typename T>
    SK_INLINE operator T(void) const
    {
        if (m_data && m_data->type() == typeid(T))
            return static_cast<ValueType<T>*>(m_data)->m_value;
        return T();
    }

    template<typename T>
    SK_INLINE T get(const T& def = T()) const
    {
        if (m_data && m_data->type() == typeid(T))
            return static_cast<ValueType<T>*>(m_data)->m_value;
        return def;
    }


    SK_INLINE bool isTypeOf(const skValue& v) const
    {
        return m_data && v.m_data && m_data->type() ==  v.m_data->type();
    }
    SK_INLINE bool isTypeOf(const Info& v) const
    {
        return m_data && m_data->type() ==  v;
    }
};
#endif
#endif//_skValue_h_
