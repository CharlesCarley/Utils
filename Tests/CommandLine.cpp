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
#include "Utils/skLogger.h"
#include "Utils/CommandLine/skCommandLineParser.h"
#include "Utils/CommandLine/skCommandLineScanner.h"
#include "catch/catch.hpp"

using namespace skCommandLine;

TEST_CASE("skCommandLineScanner_test1")
{
    const char *opts1[] = {"/user/bin/mock"};

    bool test;

    Scanner scn;
    scn.append(opts1[0]);

    Token tok;
    scn.lex(tok);

    test = tok.getType() == TOK_IDENTIFIER;
    EXPECT_TRUE(test);

    test = tok.getValue().equals("/user/bin/mock");
    EXPECT_TRUE(test);

    scn.lex(tok);
    test = tok.getType() == TOK_EOS;
    EXPECT_TRUE(test);
}

TEST_CASE("skCommandLineScanner_test2")
{
    const char *opts1[] = {"../bin/mock"};

    bool test;

    Scanner scn;
    scn.append(opts1[0]);

    Token tok;
    scn.lex(tok);

    test = tok.getType() == TOK_IDENTIFIER;
    EXPECT_TRUE(test);

    test = tok.getValue().equals("../bin/mock");
    EXPECT_TRUE(test);

    scn.lex(tok);
    test = tok.getType() == TOK_EOS;
    EXPECT_TRUE(test);
}

TEST_CASE("skCommandLineScanner_test3")
{
    const char *opts1[] = {"C:\\foo\\mock"};
    bool        test;
    Scanner     scn;
    scn.append(opts1[0]);

    Token tok;
    scn.lex(tok);

    test = tok.getType() == TOK_IDENTIFIER;
    EXPECT_TRUE(test);

    test = tok.getValue().equals("C:\\foo\\mock");
    EXPECT_TRUE(test);

    scn.lex(tok);
    test = tok.getType() == TOK_EOS;
    EXPECT_TRUE(test);
}

TEST_CASE("skCommandLineScanner_test4")
{
    // <id> <id> <-><id> <--><id>
    const char *opts1[] = {"../bin/mock", "-0e", "-fi", "--Ao"};

    Scanner scn;
    for (auto &i : opts1)
        scn.append(i);

    Token tok;
    // <id>
    scn.lex(tok);
    EXPECT_EQ(tok.getType(), TOK_IDENTIFIER);
    EXPECT_TRUE(tok.getValue().equals("../bin/mock"));

    // <id>
    scn.lex(tok);
    EXPECT_NE(tok.getType(), TOK_SWITCH_SHORT);
    EXPECT_EQ(tok.getType(), TOK_IDENTIFIER);
    EXPECT_TRUE(tok.getValue().equals("-0e"));

    // <->
    scn.lex(tok);
    EXPECT_EQ(tok.getType(), TOK_SWITCH_SHORT);
    EXPECT_TRUE(tok.getValue().empty());

    // <id>
    scn.lex(tok);
    EXPECT_EQ(tok.getType(), TOK_IDENTIFIER);
    EXPECT_TRUE(tok.getValue().equals("fi"));

    // <-->
    scn.lex(tok);
    EXPECT_EQ(tok.getType(), TOK_SWITCH_LONG);
    EXPECT_TRUE(tok.getValue().empty());
    // <id>
    scn.lex(tok);
    EXPECT_EQ(tok.getType(), TOK_IDENTIFIER);
    EXPECT_TRUE(tok.getValue().equals("Ao"));
}

TEST_CASE("skCommandLineScanner_test5")
{
    // <id> <-><id> <--><id> <id>
    const char *opts1[] = {"../bin/mock", "-h", "--abc", "123"};

    Scanner scn;
    for (auto &i : opts1)
        scn.append(i);

    Token tok;
    // <id>
    scn.lex(tok);
    EXPECT_EQ(tok.getType(), TOK_IDENTIFIER);
    EXPECT_TRUE(tok.getValue().equals("../bin/mock"));

    // <->
    scn.lex(tok);
    EXPECT_EQ(tok.getType(), TOK_SWITCH_SHORT);
    // <id>
    scn.lex(tok);
    EXPECT_EQ(tok.getType(), TOK_IDENTIFIER);
    EXPECT_TRUE(tok.getValue().equals("h"));

    // <-->
    scn.lex(tok);
    EXPECT_EQ(tok.getType(), TOK_SWITCH_LONG);
    EXPECT_TRUE(tok.getValue().empty());

    // <id>
    scn.lex(tok);
    EXPECT_EQ(tok.getType(), TOK_IDENTIFIER);
    EXPECT_TRUE(tok.getValue().equals("abc"));

    // <id>
    scn.lex(tok);
    EXPECT_EQ(tok.getType(), TOK_IDENTIFIER);
    EXPECT_TRUE(tok.getValue().equals("123"));
}

Switch Switches[] = {
    {
        0,
        0,
        "abc",
        "desc",
        true,
        1,
    },
};

TEST_CASE("skCommandLineParser_test")
{
    skLogger log;
    log.setFlags(LF_STDOUT);
    // <id> <--><id> <id>
    const char *argv[] = {"C:\\foo\\mock", "--abc", "123"};
    const int   argc   = (int)(sizeof argv / sizeof argv[0]);

    Parser p;
    int result = p.parse(argc, (char**)argv, Switches, 1);
    EXPECT_EQ(result, 0);


    EXPECT_TRUE(p.getBaseProgram().equals("mock"));
    EXPECT_TRUE(p.getProgram().equals("C:\\foo\\mock"));

    EXPECT_TRUE(p.isPresent(0));
    EXPECT_EQ(p.getValueInt(0,0), 123);

}
