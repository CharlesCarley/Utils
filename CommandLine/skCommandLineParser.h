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
#ifndef _skCommandLineParser_h_
#define _skCommandLineParser_h_

#include "Utils/CommandLine/skCommandLineOption.h"
#include "Utils/CommandLine/skCommandLineScanner.h"
#include "Utils/skArray.h"
#include "Utils/skMap.h"
#include "Utils/skString.h"

namespace skCommandLine
{
    /// <summary>
    /// Simple command line parser
    /// </summary>
    class Parser
    {
    public:
        typedef skHashTable<skString, ParseOption *> Switches;
        typedef skArray<ParseOption *>               Options;
        typedef skArray<skString>                    StringArray;

    private:
        skString    m_program;
        int         m_maxHelp;
        int         m_required;
        int         m_used;
        Scanner     m_scanner;
        Switches    m_switches;
        StringArray m_argList;
        Options     m_options;

        static int getBaseName(const char *input);

        bool hasSwitch(const skString &sw) const;

        bool initializeOption(ParseOption *opt, const Switch &sw);

        bool initializeSwitches(const Switch *switches, SKuint32 count);


    public:
        Parser();
        ~Parser();

        /// <summary>
        /// Parses the command line. Any switches that are needed must be
        /// initialized via initializeSwitches. Arguments that have no switch
        /// are placed into an array and accessed via getArgList
        /// </summary>
        /// <param name="argc">The argument count on program start.</param>
        /// <param name="argv">The argument vector on program start.</param>
        /// <param name="switches">A const array of Switch structures.</param>
        /// <param name="count">The length of the switch structure array.</param>
        /// <returns>Returns -1 on any error otherwise it returns 0</returns>
        int parse(int           argc,
                  char **       argv,
                  const Switch *switches,
                  SKuint32      count);

        /// <summary>
        /// Logs the command line verbatim
        /// </summary>
        void logInput() const;

        StringArray &getArgList()
        {
            return m_argList;
        }

        const skString &getProgram() const
        {
            return m_program;
        }

        /// <returns>The file name of the program from argv[0]</returns>
        skString getBaseProgram() const;

        


        /// <summary>
        /// Extracts the directory from the supplied path to main.
        /// </summary>
        /// <returns>The directory name of the program from argv[0]</returns>
        skString getProgramDirectory() const;


        /// <summary>
        /// Returns the current working directory for this process.
        /// </summary>
        /// <returns>The current working directory.</returns>
        static skString getCurrentWorkingDirectory();


        /// <param name="enumId">The switch id</param>
        /// <returns>true if it is supplied on the command line false otherwise</returns>
        bool isPresent(const SKuint32 &enumId) const;

        /// <param name="enumId">The switch id</param>
        /// <returns> the option at the enumId or null if the id is out of bounds</returns>
        ParseOption *getOption(const SKuint32 &enumId);

        SKint32 getValueInt(const SKuint32 &enumId,
                            SKsize          idx,
                            SKint32         defaultValue = -1,
                            SKint32         base         = 10) const;

        SKint64 getValueInt64(const SKuint32 &enumId,
                              SKsize          idx,
                              SKint64         defaultValue = -1,
                              SKint32         base         = 10) const;

        const skString &getValueString(const SKuint32 &enumId,
                                       SKsize          idx,
                                       const skString &defaultValue = "") const;

        /// <summary>
        /// Prints a usage message with the supplied switch information
        /// </summary>
        void usage();
    };

}  // namespace skCommandLine

#endif  // !_skCommandLineOption_h_
