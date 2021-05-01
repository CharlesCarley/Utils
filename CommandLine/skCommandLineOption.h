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
#ifndef _skCommandLineOption_h_
#define _skCommandLineOption_h_

#include "Utils/skArray.h"
#include "Utils/skString.h"

namespace skCommandLine
{
    /// <summary>
    /// Represents a command line switch
    /// </summary>
    struct Switch
    {
        /// This needs to be it's index in the
        /// context of all defined switches
        SKuint32 id;

        /// Short switch (-[a-zA-Z]+)
        /// A value of 0 means unused
        char shortSwitch;

        // Long switch (--[.]+)
        // A value of null means unused
        const char* longSwitch;

        /// A brief description for the -h, --help builtin option
        const char* description;

        /// Means it's an optional switch
        bool optional;

        /// Defines the number of required arguments
        int argCount;
    };

    class ParseOption
    {
    public:
        typedef skArray<skString> Arguments;

    private:
        const Switch m_switch{};

        Arguments m_value;
        bool      m_isPresent;

    public:
        ParseOption() :
            m_isPresent(false)
        {
        }

        explicit ParseOption(const Switch& sw) :
            m_switch(sw),
            m_isPresent(false)
        {
            if (sw.argCount > 0)
                m_value.resize((SKint32)sw.argCount);
        }

        bool isOptional() const
        {
            return m_switch.optional;
        }

        bool isPresent() const
        {
            return m_isPresent;
        }

        SKsize getArgCount() const
        {
            return m_value.size();
        }

        void makePresent()
        {
            m_isPresent = true;
        }

        const Switch& getSwitch() const
        {
            return m_switch;
        }

        const Arguments& getValues() const
        {
            return m_value;
        }

        const skString& getValue(const SKsize idx = 0)
        {
            SK_ASSERT(idx <= 0 && idx < m_value.size());
            return m_value[(SKuint32)idx];
        }

        void setValue(const skString& str)
        {
            if (!m_value.empty())
                m_value[0] = str;
        }

        int getInt(SKsize idx = 0, int defaultValue = -1, int base = 10)
        {
            return skChar::toInt32(getValue(idx),
                                   defaultValue,
                                   base);
        }

        SKint64 getInt64(SKsize idx = 0, SKint64 defaultValue = -1, int base = 10)
        {
            return skChar::toInt64(getValue(idx),
                                   defaultValue,
                                   base);
        }

        void setValue(SKsize i, const skString& str)
        {
            if (i < m_value.size())
                m_value[(SKint32)i] = str;
        }
    };

}  // namespace skCommandLine

#endif  // !_skCommandLineOption_h_
