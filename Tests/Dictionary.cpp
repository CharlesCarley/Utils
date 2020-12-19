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
#include "Utils/skDictionary.h"
#include "Utils/skString.h"
#include "catch/catch.hpp"

typedef skDictionary<skString, int> Map;

void skDictionary_populate(Map& map)
{
    int i = 1;
    map.insert("Jane", i++);
    map.insert("John", i++);
    map.insert("Bob", i++);
    map.insert("Bill", i++);
    map.insert("Mary", i++);
}

TEST_CASE("Dictionary_insert")
{
    Map dict;
    skDictionary_populate(dict);
    REQUIRE(5 == dict.size());

    dict.insert("Mary", 7);
    dict.insert("Bill", 7);
    REQUIRE(5 == dict.size());
}

TEST_CASE("Dictionary_remove")
{
    Map dict;
    skDictionary_populate(dict);
    REQUIRE(5 == dict.size());

    bool val = dict.has_key("Bill");
    REQUIRE(val);
    dict.erase("Bill");

    val = dict.has_key("Bill");
    REQUIRE(false == val);
    REQUIRE(4 == dict.size());
}

TEST_CASE("Dictionary_iterators")
{
    Map dict;
    skDictionary_populate(dict);
    REQUIRE(5 == dict.size());

    int i = 0;

    bool val = dict.has_key("Bill");
    REQUIRE(true == val);
    dict.erase("Bill");

    val = dict.has_key("Bill");
    REQUIRE(false == val);

    Map::Iterator it = dict.iterator();
    while (it.hasMoreElements())
    {
        Map::ConstReferenceType mval = it.getNext();
        REQUIRE(i < mval.second);
        ++i;
    }
}
