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
#include <limits>
#include "Macro.h"
#include "Utils/skHexPrint.h"
#include "Utils/skString.h"
#include "Utils/skStringConverter.h"
#include "catch/catch.hpp"

TEST_CASE("StringTest_BasicString")
{
    skString a = "Hello World";
    skString b = a;
    skString c = b;
    EXPECT_EQ(b, a);
    EXPECT_EQ(c, a);
}

TEST_CASE("StringTest_FindString")
{
    skString a   = "Hello World";
    SKsize   pos = a.find("World");
    REQUIRE(a.npos != pos);
    REQUIRE(skStringUtils::length("Hello ") == pos);
}

TEST_CASE("StringTest_SplitString")
{
    skString a = "Hello World";

    skStringArray spl;
    a.split(spl, " ");

    REQUIRE(2 == spl.size());

    bool s0eq = spl[0] == skString("Hello");
    bool s1eq = spl[1] == skString("World");

    REQUIRE(s0eq == true);
    REQUIRE(s1eq == true);
}

TEST_CASE("StringTest FormatString1")
{
    const skString a = skString::format("%s %s", "Hello", "World");
    REQUIRE(a.size() == 11);

    skStringArray spl;
    a.split(spl, " ");

    REQUIRE(2 == spl.size());

    bool s0eq = spl[0] == skString("Hello");
    bool s1eq = spl[1] == skString("World");

    REQUIRE(s0eq == true);
    REQUIRE(s1eq == true);
}

TEST_CASE("StringTest FormatString2")
{
    skString a;
    skString::format(a, "%s %s", "Hello", "World");
    REQUIRE(a.size() == 11);

    skStringArray spl;
    a.split(spl, " ");

    REQUIRE(2 == spl.size());

    bool s0eq = spl[0] == skString("Hello");
    bool s1eq = spl[1] == skString("World");

    REQUIRE(s0eq == true);
    REQUIRE(s1eq == true);
}

int STR_Greater(char a, char b)
{
    return a > b;
}

int STR_Less(char a, char b)
{
    return a < b;
}

TEST_CASE("StringTest_SortString")
{
    skString a = "0123456789abcdefghijklmnopqrstuvwxyz";
    a.sort(STR_Greater);

    SKsize                i = 0;
    const SKsize          s = a.size() - 1;
    skString::PointerType p = a.ptr();
    while (i < s)
    {
        int sa = p[i];
        int sb = p[i + 1];

        if (DEBUG)
            printf("STR %c GE %c | (%i, %i)\n", sa, sb, sa, sb);

        REQUIRE(sa > sb);
        i += 2;
    }

    a.sort(STR_Less);
    i = 0;

    while (i < s)
    {
        int sa = p[i];
        int sb = p[i + 1];
        if (DEBUG)
            printf("STR %c LE %c | (%i, %i)\n", sa, sb, sa, sb);
        REQUIRE(sa < sb);
        i += 2;
    }
}

TEST_CASE("StringTest_AddString")
{
    skString a;

    for (SKsize i = 0; i < 5; ++i)
        a += "Hello ";

    skStringArray spl;
    a.split(spl, " ");

    REQUIRE(5 == spl.size());
}

TEST_CASE("StringTest_CopyString")
{
    skString a = skString("Test String...");

    char   buffer[20];
    SKsize len  = a.copy(buffer, 6, 5);
    buffer[len] = 0;

    bool eq = skStringUtils::equals(buffer, "String") == 0;
    REQUIRE(eq);

    skString b;
    len = b.copy(buffer, 6, 5);
    REQUIRE(0 == len);

    skString c;
    len = c.copy(nullptr, 6, 5);
    REQUIRE(0 == len);

    len = a.copy(nullptr, a.size() - 1, 1);
    REQUIRE(0 == len);

    skString d  = "Test String...";
    len         = d.copy(buffer, d.size(), 0);
    buffer[len] = 0;
    REQUIRE(d.size() == len);

    eq = skStringUtils::equals(buffer, "Test String...") == 0;
    REQUIRE(eq);
}

TEST_CASE("StringTest_SubString")
{
    // http://www.cplusplus.com/reference/string/string/substr/

    skString a = "We think in generalities, but we live in details.";
    skString b, c, d;

    SKsize loc;

    b = a.substr(12, 12);  // "generalities"

    bool eq = b == "generalities";
    REQUIRE(eq);

    loc = a.find("live");    // position of "live" in str
    c   = a.substr(loc, 0);  // get from "live" to the end

    d  = b + skString(" ") + c;
    eq = d == "generalities live in details.";
    REQUIRE(eq);

    a = "Test String...";
    b = a.substr(a.size(), 0);
    REQUIRE(b.empty());

    c  = a.substr(a.size() / 2, a.size());
    eq = c == "ring...";
    REQUIRE(eq);
}

TEST_CASE("StringTest_EraseString")
{
    skString a = "This is an example phrase.";
    a.erase(10, 8);

    bool eq = a == "This is an phrase.";
    REQUIRE(eq);

    a.erase(5, 6);
    eq = a == "This phrase.";
    REQUIRE(eq);

    a.erase(a.size(), a.size());
    eq = a == "This phrase.";
    REQUIRE(eq);

    a.erase(0, a.size());
    REQUIRE(a.empty());

    skString ee;
    ee.erase(0, 30);
}

TEST_CASE("StringTest_NullStringTests")
{
    skString a, b;
    a.erase(10, 8);
    SKsize pos = a.find("123");
    EXPECT_EQ(pos, SK_NPOS);

    a = a.substr(0, 8);
    EXPECT_EQ(a.size(), 0);

    a = b;
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.ptr(), nullptr);

    char c = a[0];
    EXPECT_EQ(c, 0);
    char d = b[0];
    EXPECT_EQ(d, 0);
    char e = a.at(0);
    EXPECT_EQ(e, 0);
    char f = b.at(0);
    EXPECT_EQ(f, 0);

    a.append(b);
    a = a + b;
    EXPECT_EQ(a.size(), 0);

    skString v = "123";
    v += a + b;

    skStringArray arr;
    a.split(arr, "123");
    EXPECT_EQ(arr.size(), 0);
}

TEST_CASE("StringTest_EncriptDecript")
{
    skString a = "Caesar cipher variation.";
    int      i;

    SKbyte   lowerBits[4] = {};
    SKuint16 upperBits[4] = {};

    for (i = 0; i < 4; ++i)
        lowerBits[i] = (SKbyte)rand() % 128;
    for (i = 0; i < 4; ++i)
        upperBits[i] = (SKuint16)rand() % 65536;

    a.encrypt(lowerBits, 4, upperBits, 4);
    skHexPrint::dumpHex((void*)a.c_str(), 0, a.size());

    a.decrypt(lowerBits, 4, upperBits, 4);
    skHexPrint::dumpHex((void*)a.c_str(), 0, a.size());

    REQUIRE(a == "Caesar cipher variation.");
}

TEST_CASE("StringTest_BinaryStringRepr")
{
    skString a = "Binary";
    a.toBinary();
    if (DEBUG)
        printf("%s\n", a.c_str());
    skString b = a;
    b.fromBinary();
    REQUIRE(b == "Binary");
}

TEST_CASE("StringTest_HexStringRepr")
{
    skString a = "Hex string representation";
    a.toHex();
    if (DEBUG)
        printf("%s\n", a.c_str());
    skString b = a;
    b.fromHex();
    REQUIRE(b == "Hex string representation");
}

using Convert = skStringConverter;

TEST_CASE("StringConverter bool")
{
    bool test;

    test = Convert::toBool("Hello World");
    EXPECT_EQ(false, test);

    test = Convert::toBool("true");
    EXPECT_EQ(true, test);

    test = Convert::toBool("false");
    EXPECT_EQ(false, test);

    test = Convert::toBool("yes");
    EXPECT_EQ(true, test);

    test = Convert::toBool("no");
    EXPECT_EQ(false, test);

    test = Convert::toBool("1");
    EXPECT_EQ(true, test);
}

TEST_CASE("StringConverter int16")
{
    SKint16 test;
    test = Convert::toInt16("Hello World");
    EXPECT_EQ(0, test);

    test = Convert::toInt16("-32768");
    EXPECT_EQ(std::numeric_limits<SKint16>::min(), test);

    test = Convert::toInt16("3276999");
    EXPECT_EQ(std::numeric_limits<SKint16>::max(), test);

    test = Convert::toInt16("-987532767");
    EXPECT_EQ(std::numeric_limits<SKint16>::min(), test);

    test = Convert::toInt16("-2147483648087655446879898769876576");
    EXPECT_EQ(std::numeric_limits<SKint16>::min(), test);
}

TEST_CASE("StringConverter uint16")
{
    SKuint16 test;
    test = Convert::toUint16("Hello World");
    EXPECT_EQ(0, test);

    test = Convert::toUint16("-65536");
    EXPECT_EQ(0xFFFF, test);

    test = Convert::toUint16("65536");
    EXPECT_EQ(0xFFFF, test);

    test = Convert::toUint16("-987532767");
    EXPECT_EQ(0xFFFF, test);

    test = Convert::toUint16("2147483648087655446879898769876576");
    EXPECT_EQ(0xFFFF, test);

    test = Convert::toUint16("65534");
    EXPECT_EQ(0xFFFE, test);
}

TEST_CASE("StringConverter int32")
{
    int test;
    test = Convert::toInt32("Hello World");
    EXPECT_EQ(0, test);

    test = Convert::toInt32("-2147483648");
    EXPECT_EQ(std::numeric_limits<int>::min(), test);

    test = Convert::toInt32("2147483647");
    EXPECT_EQ(std::numeric_limits<int>::max(), test);

    test = Convert::toInt32("2147483648087655446879898769876576");
    EXPECT_EQ(std::numeric_limits<int>::max(), test);

    test = Convert::toInt32("-2147483648087655446879898769876576");
    EXPECT_EQ(std::numeric_limits<int>::min(), test);
}

TEST_CASE("StringConverter uint32")
{
    SKuint32 test;
    test = Convert::toUint32("Hello World");
    EXPECT_EQ(0, test);

    test = Convert::toUint32("-4294967294");
    EXPECT_EQ(0xFFFFFFFF, test);

    test = Convert::toUint32("4294967294");
    EXPECT_EQ(0xFFFFFFFE, test);

    test = Convert::toUint32("2147483648087655446879898769876576");
    EXPECT_EQ(0xFFFFFFFF, test);

    test = Convert::toUint32("-2147483648087655446879898769876576");
    EXPECT_EQ(0xFFFFFFFF, test);
}

TEST_CASE("StringConverter int64")
{
    SKint64 test;
    test = Convert::toInt32("Hello World");
    EXPECT_EQ(0, test);

    test = Convert::toInt64("-18446744073709551616");
    EXPECT_EQ(std::numeric_limits<SKint64>::min(), test);

    test = Convert::toInt64("18446744073709551617");
    EXPECT_EQ(std::numeric_limits<SKint64>::max(), test);

    test = Convert::toInt64("2147483648087655446879898769876576");
    EXPECT_EQ(std::numeric_limits<SKint64>::max(), test);

    test = Convert::toInt64("-2147483648087655446879898769876576");
    EXPECT_EQ(std::numeric_limits<SKint64>::min(), test);
}

TEST_CASE("StringConverter uint64")
{
    SKuint64 test;
    test = Convert::toUint64("Hello World");
    EXPECT_EQ(0, test);

    test = Convert::toUint64("-18446744073709551616");
    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, test);

    test = Convert::toUint64("18446744073709551616");
    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, test);

    test = Convert::toUint64("18446744073709551614");
    EXPECT_EQ(0xFFFFFFFFFFFFFFFE, test);

    test = Convert::toUint64("2147483648087655446879898769876576");
    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, test);

    test = Convert::toUint64("-2147483648087655446879898769876576");
    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, test);
}

TEST_CASE("StringConverter toInt16")
{
    {
        const skString iString = "123";
        SKint16        v       = iString.toInt16();
        EXPECT_EQ(123, v);

        skString r;
        skChar::toString(r, v);
        EXPECT_EQ(3, r.size());
        EXPECT_TRUE(skChar::equalsn("123", r.c_str(), 3) == 0);
    }

    {
        const skString iString = "9985287698765975976";
        SKint16        v       = iString.toInt16();
        EXPECT_EQ(32767, v);

        skString r;
        skChar::toString(r, v);
        EXPECT_EQ(5, r.size());
        EXPECT_TRUE(skChar::equalsn("32767", r.c_str(), 5) == 0);
    }

    {
        const skString iString = "-9985287698765975976";
        SKint16        v       = iString.toInt16();
        EXPECT_EQ(-32767-1, v);

        skString r;
        skChar::toString(r, v);
        EXPECT_EQ(6, r.size());
        EXPECT_TRUE(skChar::equalsn("-32768", r.c_str(), 6) == 0);
    }
}

TEST_CASE("StringConverter toInt32")
{
    {
        const skString iString = "123";
        SKint32        v       = iString.toInt32();
        EXPECT_EQ(123, v);

        skString r;
        skChar::toString(r, v);
        EXPECT_EQ(3, r.size());
        EXPECT_TRUE(skChar::equalsn("123", r.c_str(), 3) == 0);
    }

    {
        const skString iString = "9985287698765975976";
        SKint32        v       = iString.toInt32();
        EXPECT_EQ(2147483647, v);

        skString r;
        skChar::toString(r, v);
        EXPECT_EQ(10, r.size());
        EXPECT_TRUE(skChar::equalsn("2147483647", r.c_str(), 5) == 0);
    }

    {
        const skString iString = "-9985287698765975976";
        SKint32        v       = iString.toInt32();
        EXPECT_EQ(-2147483647-1, v);

        skString r;
        skChar::toString(r, v);
        EXPECT_EQ(11, r.size());
        EXPECT_TRUE(skChar::equalsn("-2147483648", r.c_str(), 11) == 0);
    }
}


TEST_CASE("StringConverter toInt64")
{
    {
        const skString iString = "123";
        SKint64        v       = iString.toInt64();
        EXPECT_EQ(123, v);

        skString r;
        skChar::toString(r, v);
        EXPECT_EQ(3, r.size());
        EXPECT_TRUE(skChar::equalsn("123", r.c_str(), 3) == 0);
    }

    {
        const skString iString = "9985287698765975988875454545476";
        SKint64        v       = iString.toInt64();
        EXPECT_EQ(9223372036854775807, v);

        skString r;
        skChar::toString(r, v);
        EXPECT_EQ(19, r.size());
        EXPECT_TRUE(skChar::equalsn("9223372036854775807", r.c_str(), 19) == 0);
    }

    {
        const skString iString = "-9985287698765975988875454545476";
        SKint64        v       = iString.toInt64();
        EXPECT_EQ(-9223372036854775807-1, v);

        skString r;
        skChar::toString(r, v);
        EXPECT_EQ(20, r.size());
        EXPECT_TRUE(skChar::equalsn("-9223372036854775808", r.c_str(), 20) == 0);
    }
}
