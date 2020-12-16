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
#include "skCommandLineParser.h"
#include "Utils/skDebugger.h"
#include "Utils/skLogger.h"

using namespace skCommandLine;

void Parser_logWhiteSpace(int nr);

Parser::Parser() :
    m_maxHelp(0),
    m_required(0),
    m_used(0)
{
}

Parser::~Parser()
{
    Options::Iterator it = m_options.iterator();
    while (it.hasMoreElements())
        delete it.getNext();
}

int Parser::getBaseName(const char *input) const
{
    int offs = 0;
    if (input)
    {
        const int len = (int)skStringUtils::length(input);

        int i;
        for (i = len - 1; i >= 0 && offs == 0; --i)
            if (input[i] == '/' || input[i] == '\\')
                offs = i + 1;
    }
    return offs;
}

bool Parser::hasSwitch(const skString &sw) const
{
    return m_switches.find(sw) != SK_NPOS;
}

int Parser::parse(int           argc,
                  char **       argv,
                  const Switch *switches,
                  SKsize        count)
{
    if (!m_program.empty())  // using as a check for multiple calls
        return 0;

    if (!initializeSwitches(switches, count))
        return -1;

    m_program.append(argv[0]);
    m_used = 0;
    m_scanner.clear();

    // it's easier to parse it as a single string
    for (int i = 1; i < argc; ++i)
        m_scanner.append(argv[i]);

    skString curSwitch;
    curSwitch.reserve(16);

    Token a;
    while (a.getType() != TOK_EOS)
    {
        m_scanner.lex(a);
        const int type = a.getType();

        if (a.getType() == TOK_ERROR)
        {
            skLogf(LD_ERROR, "\nunknown error %s\n\n", a.getValue().c_str());
            usage();
            return -1;
        }

        if (type == TOK_SWITCH_SHORT || type == TOK_SWITCH_LONG)
        {
            // grab the next token
            m_scanner.lex(a);

            if (a.getType() == TOK_NEXT || a.getType() == TOK_EOS)
            {
                skLogf(LD_ERROR, "expected a switch value to follow '-'\n");
                usage();
                return -1;
            }
            if (a.getValue().equals("help") || a.getValue().equals("h"))
            {
                usage();
                return -1;
            }

            // > next && < TOK_EOS

            const SKsize pos = m_switches.find(a.getValue());
            if (pos == SK_NPOS)
            {
                skLogf(LD_ERROR, "Unknown option '%s'\n", a.getValue().c_str());
                usage();
                return -1;
            }

            curSwitch.assign(a.getValue());

            ParseOption *opt = m_switches.at(pos);
            opt->makePresent();

            if (!opt->isOptional())
                m_used++;

            if (opt->getArgCount() > 0)
            {
                // parse
                SKsize i;
                for (i = 0; i < opt->getArgCount(); ++i)
                {
                    m_scanner.lex(a);
                    if (a.getValue().empty())
                    {
                        skLogf(LD_ERROR,
                               "missing argument for option '%s'\n",
                               curSwitch.c_str());
                        usage();
                        return -1;
                    }

                    opt->setValue(i, a.getValue());
                }

                if (i != opt->getArgCount())
                {
                    skLogf(LD_ERROR,
                           "not all arguments converted when parsing switch '%s'\n",
                           curSwitch.c_str());
                    usage();
                    return -1;
                }
            }
        }
        else if (type >= TOK_OPTION && type < TOK_EOS)
            m_argList.push_back(a.getValue());
        else
        {
            if (type != TOK_EOS)
            {
                skLogf(LD_ERROR, "unknown option '%s'\n", a.getValue().c_str());
                usage();
                return -1;
            }
        }
    }

    if (m_used != m_required)
    {
        skLogf(LD_ERROR, "missing required options.\n");
        usage();
        return -1;
    }
    return 0;
}

void Parser::logInput()
{
    skLogf(LD_INFO,
           "\n ~> %s %s\n\n",
           getBaseProgram().c_str(),
           m_scanner.getValue().c_str());
}

skString Parser::getBaseProgram() const
{
    skString returnValue;
    m_program.substr(returnValue,
                     getBaseName(m_program.c_str()),
                     m_program.size());
    return returnValue;
}

bool Parser::isPresent(const SKuint32 &enumId)
{
    if (enumId < m_options.size())
        return m_options[enumId]->isPresent();
    return false;
}

ParseOption *Parser::getOption(const SKuint32 &enumId)
{
    if (enumId < m_options.size())
        return m_options[enumId];
    return nullptr;
}

SKint32 Parser::getValueInt(const SKuint32 &enumId,
                            SKsize          idx,
                            SKint32         defaultValue,
                            SKint32         base) const
{
    if (enumId < m_options.size())
    {
        if (m_options[enumId] != nullptr)
            return m_options[enumId]->getInt(idx, defaultValue, base);
    }
    return defaultValue;
}

SKint64 Parser::getValueInt64(const SKuint32 &enumId,
                              SKsize          idx,
                              SKint64         defaultValue,
                              SKint32         base) const
{
    if (enumId < m_options.size())
    {
        if (m_options[enumId] != nullptr)
            return m_options[enumId]->getInt64(idx, defaultValue, base);
    }
    return defaultValue;
}

const skString &Parser::getValueString(const SKuint32 &enumId,
                                       SKsize          idx,
                                       const skString &defaultValue) const
{
    if (enumId < m_options.size())
    {
        if (m_options[enumId] != nullptr)
            return m_options[enumId]->getValue(idx);
    }
    return defaultValue;
}

void Parser::usage()
{
    skDebugger::writeColor(CS_DARKYELLOW);
    logInput();
    skDebugger::writeColor(CS_WHITE);

    skLogf(LD_INFO, "Usage: %s <options>\n\n", getBaseProgram().c_str());
    skLogf(LD_INFO, "  <options>: ");
    skLogf(LD_INFO, "<!> == required\n\n");
    skLogf(LD_INFO, "    -h, --help");

    Parser_logWhiteSpace(m_maxHelp - 4);
    skLogf(LD_INFO, " Display this help message.\n");

    Options::Iterator it = m_options.iterator();
    while (it.hasMoreElements())
    {
        ParseOption * opt = it.getNext();
        const Switch &sw  = opt->getSwitch();

        if (!sw.optional)
            skLogf(LD_INFO, "!, ");
        else
            skLogf(LD_INFO, "    ");

        if (sw.shortSwitch != 0)
        {
            skLogf(LD_INFO, "-%c", sw.shortSwitch);
            if (sw.longSwitch != nullptr)
                skLogf(LD_INFO, ", ");
            else
                skLogf(LD_INFO, "  ");
        }
        else
            skLogf(LD_INFO, "    ");

        int space = m_maxHelp;
        if (sw.longSwitch != nullptr)
        {
            space -= (int)skStringUtils::length(sw.longSwitch);
            skLogf(LD_INFO, "--%s", sw.longSwitch);
        }

        Parser_logWhiteSpace(space);

        skDebugger::writeColor(CS_LIGHT_GREY);

        if (sw.description != nullptr)
        {
            skStringArray arr;
            skString      str(sw.description);
            str.split(arr, "\n");

            for (SKsize i = 0; i < arr.size(); ++i)
            {
                skLogf(LD_INFO, " %s", arr[i].c_str());

                if (i + 1 < arr.size())
                {
                    skLogf(LD_INFO, "\n");
                    Parser_logWhiteSpace(m_maxHelp+10);
                }
            }

            if (arr.size() > 1)
                skLogf(LD_INFO, "\n");
        }
        skDebugger::writeColor(CS_WHITE);
        skLogf(LD_INFO, "\n");
    }

    skLogf(LD_INFO, "\n");
}

bool Parser::initializeOption(ParseOption *opt, const Switch &sw)
{
    if (sw.shortSwitch == 0 && sw.longSwitch == nullptr)
    {
        skLogf(LD_ERROR,
               "A switch must have at least a long or short name\n");
        return false;
    }

    if (sw.shortSwitch != 0)
    {
        if (hasSwitch(skString(sw.shortSwitch, 1)))
        {
            skLogf(LD_ERROR, "Duplicate switch '-%c'\n", sw.shortSwitch);
            return false;
        }
    }

    if (sw.longSwitch != nullptr)
    {
        const skString lsw = skString(sw.longSwitch);

        m_maxHelp = skMax(m_maxHelp, (int)lsw.size());

        if (hasSwitch(lsw))
        {
            skLogf(LD_ERROR, "Duplicate switch '--%s'\n", sw.longSwitch);
            return false;
        }
    }

    // cross reference short and long switch names.
    // the option can be accessed by looking up
    // via the short, long switch or directly by it's
    // id defined in Switch::id

    if (sw.shortSwitch != 0)
        m_switches.insert(skString(sw.shortSwitch, 1), opt);
    if (sw.longSwitch != nullptr)
        m_switches.insert(sw.longSwitch, opt);

    return true;
}

bool Parser::initializeSwitches(const Switch *switches, SKsize count)
{
    if (switches == nullptr || count == 0)
        return true;

    count = skMin<SKsize>(count, 0x100);

    m_options.resize(count);
    bool result = true;

    for (SKsize i = 0; i < count && result; ++i)
    {
        if (switches[i].id != i)
        {
            skLogf(LD_ERROR, "Misaligned switch id.\n");
            result = false;
        }
        else
        {
            if (!switches[i].optional)
                m_required++;

            m_options[i] = new ParseOption(switches[i]);

            result = initializeOption(m_options[i], switches[i]);
        }
    }
    return result;
}

void Parser_logWhiteSpace(int nr)
{
    while (nr--)
        skLogf(LD_INFO, " ");
}
