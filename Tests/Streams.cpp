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
#include "Macro.h"
#include "catch/catch.hpp"
#include "Utils/skFileStream.h"
#include "Utils/skString.h"

#include "TestConfig.h"

#define FileName(x) TestDirectory x


TEST_CASE("FileStream OpenRead")
{
    skFileStream fs(FileName("/TestFiles/Test1.txt"), skStream::READ);

    EXPECT_EQ(true, fs.isOpen());
    EXPECT_EQ(true, fs.canRead());
    EXPECT_EQ(false, fs.canWrite());
    EXPECT_EQ(11, fs.size());
}


TEST_CASE("FileStream OpenWrite")
{
    skFileStream fs(FileName("/TestFiles/Test2.txt"), skStream::WRITE);

    EXPECT_EQ(true, fs.isOpen());
    EXPECT_EQ(false, fs.canRead());
    EXPECT_EQ(true, fs.canWrite());
    EXPECT_EQ(0, fs.size());

    SKsize bw = fs.writef("Hello World");
    EXPECT_EQ(bw, fs.size());
    EXPECT_EQ(bw, 11);
    EXPECT_EQ(bw, fs.position());
    SKsize bw2 = fs.writef("Hello World");
    EXPECT_EQ(bw2, fs.size() - bw);
    EXPECT_EQ(bw2, 11);
    EXPECT_EQ(bw*2, fs.position());
}




TEST_CASE("FileStream WriteF")
{
    skFileStream fs(FileName("/TestFiles/Test3.txt"), skStream::WRITE);

    EXPECT_EQ(true, fs.isOpen());
    EXPECT_EQ(false, fs.canRead());
    EXPECT_EQ(true, fs.canWrite());
    EXPECT_EQ(0, fs.size());

    SKsize bw = fs.writef("Hello %s", "World");
    EXPECT_EQ(bw, fs.size());
    EXPECT_EQ(bw, 11);
    EXPECT_EQ(bw, fs.position());
    SKsize bw2 = fs.writef("Hello %s", "World");
    EXPECT_EQ(bw2, fs.size() - bw);
    EXPECT_EQ(bw2, 11);
    EXPECT_EQ(bw * 2, fs.position());
}



TEST_CASE("FileStream Test3 - Read")
{
    skFileStream fs(FileName("/TestFiles/Test3.txt"), skStream::READ);

    EXPECT_EQ(true, fs.isOpen());
    EXPECT_EQ(true, fs.canRead());
    EXPECT_EQ(false, fs.canWrite());
    EXPECT_EQ(22, fs.size());

    SKsize bw, br;
    
    bw = fs.writef("Hello %s", "World");
    EXPECT_EQ(bw, SK_NPOS);

    bw = fs.write(nullptr, -1);
    EXPECT_EQ(bw, SK_NPOS);
    EXPECT_EQ(0, fs.position());

    char block[23];
    br = fs.read(block, 5);
    EXPECT_EQ(br, 5);
    block[br] = 0;

    EXPECT_EQ(skString(block), "Hello");

    br = fs.read(block, SK_NPOS);
    EXPECT_EQ(22-br, 5);
}
