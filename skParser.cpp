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
#include "skParser.h"
#include "skDebugger.h"
#include "skFixedString.h"
#include "skStreams.h"
#include "skValue.h"

#define SK_PARSER_TRACE 1
#if SK_PARSER_TRACE
#define skParserTrace skPrintf
#define skREAD "Token Read"
#define skSHIFT "Shift"
#define skREDUCE "Reduce"
#define skFMT "%-11s|   "
#else
#define skParserTrace(x, y, z, w) (void)0
#endif

#define skLabel(x) \
    x:
#define skJmp(x) goto x
#define skDFAedge(s, e) m_dfa->m_states[s].m_edges[e]
#define skChSt(s, e) m_charsets->m_chars[skDFAedge(s, e).m_char]
#define skLALR(idx) m_lalr->m_states[idx]
#define skTmpBuf skString

#define skBufferComment(b, x) (((b[x] != '\n' && b[x - 1] != '\r') && (b[x] != '\r') && (b[x] != '\n')))

skParser::skParser() :
    m_stream(0),
    m_start(-1),
    m_symbolTable(0),
    m_ruleTable(0),
    m_dfa(0),
    m_lalr(0),
    m_buffer(0),
    m_len(0),
    m_stateIdx(0),
    m_cur(0)

{
}

skParser::~skParser()
{
    if (m_buffer)
    {
        delete[] m_buffer;
        m_buffer = 0;
    }
}

void skParser::initialize(SKuint32 stackSize, skStream* in)
{
    m_stream = in;
    if (!m_stream || !m_stream->isOpen() || m_start == -1)
        return;

    m_cur = 0;
    m_input.reserve(32);
    m_stack.reserve(stackSize);

    m_len = in->size();
    m_stateIdx = m_lalr->m_initial;

    StackItem item;
    item.m_state = m_stateIdx;
    item.m_symbol = &m_symbolTable->m_symbols[m_start];
    item.m_ptr = 0;
    item.m_primitive.ival = 0;

    m_stack.push(item);

    m_buffer = new char[m_len + 1];
    m_stream->read(m_buffer, m_len);
    m_buffer[m_len] = 0;
}

skParser::Message skParser::parseImpl(void)
{
    if (!m_buffer || m_len <= 0)
        return MESSAGE_NOT_LOADED_ERROR;

    SKint16               dfa_state, dfa_accept, dfa_len, char_set, cntr, dfa_prev;
    SKint16               dfa_cs, c1, c2;
    skParser::LALRAction* action = 0;
    StackItem             item;
    item.m_ptr = 0;
    char*    dfa_buf;
    skTmpBuf tmpBuf;

    // ----------------------------------------------------
    skLabel(scnrRead);

    dfa_len = 0x00;
    dfa_accept = 0xFF;
    dfa_prev = 0xFF;
    dfa_state = m_dfa->m_initial;
    dfa_buf = &m_buffer[m_cur];

    if ((*dfa_buf) == 0x00)
    {
        dfa_accept = 0;
        skJmp(dfaAccept);
    }

    // ----------------------------------------------------
    skLabel(dfaScan);

    if (m_dfa->m_states[dfa_state].m_accept != 0xFF)
        dfa_accept = m_dfa->m_states[dfa_state].m_accept;

    dfa_cs = dfa_buf[dfa_len];
    cntr = 0x0000;
    while (cntr < m_dfa->m_states[dfa_state].m_edgeCount)
    {
        char_set = 0x0000;
        while (char_set < skChSt(dfa_state, cntr).m_count)
        {
            if (m_charsets->m_isExpanded)
            {
                if (dfa_cs == skChSt(dfa_state, cntr).m_chars[char_set++])
                {
                    dfa_prev = dfa_accept;
                    dfa_state = (skDFAedge(dfa_state, cntr).m_target);
                    dfa_len++;
                    skJmp(dfaScan);
                }
            }
            else
            {
                c1 = skChSt(dfa_state, cntr).m_chars[char_set + 0];
                c2 = skChSt(dfa_state, cntr).m_chars[char_set + 1];
                char_set += 2;

                if ((c2 > c1 && dfa_cs >= c1 && dfa_cs <= c2) || dfa_cs == c1)
                {
                    dfa_prev = dfa_accept;
                    dfa_state = (skDFAedge(dfa_state, cntr).m_target);
                    dfa_len++;
                    skJmp(dfaScan);
                }
            }
        }

        cntr++;
    }

    // ----------------------------------------------------
    skLabel(dfaAccept);

    if (dfa_accept != 0xFF && dfa_accept > 0)
    {
        if (m_symbolTable->m_symbols[dfa_accept].m_kind == SM_EOF)
            return MESSAGE_ACCEPT;
        else if (m_symbolTable->m_symbols[dfa_accept].m_kind == SM_WHITESPACE)
        {
            m_cur += dfa_len;
            skJmp(scnrRead);
        }
        else if (m_symbolTable->m_symbols[dfa_accept].m_kind == SM_COMMENT_LINE)
        {
            m_cur += dfa_len;

            do
            {
                m_cur++;
            } while (skBufferComment(m_buffer, m_cur) && m_buffer[m_cur] != 0);
            skJmp(scnrRead);
        }
        else if (m_symbolTable->m_symbols[dfa_accept].m_kind == SM_COMMENT_START)
        {
            SKint32 out = -1;

            m_cur = handleBlockComment(m_buffer, m_cur, m_len - 1, out, &tmpBuf);

            if (m_cur == -1)
                skJmp(psrSyntax);

            if (out != -1)
            {
                m_input.push(out);
                skJmp(lalrAction);
            }

            skJmp(scnrRead);
        }
        else
            tmpBuf = skTmpBuf(dfa_buf, dfa_len);

        m_cur += dfa_len;
    }
    else
        return MESSAGE_ACCEPT;

    m_input.push(dfa_accept);

    // ----------------------------------------------------
    skLabel(lalrAction);

    cntr = 0x0000;
    while (cntr < skLALR(m_stateIdx).m_count)
    {
        action = &skLALR(m_stateIdx).m_action[cntr];

        if (action->m_symbolIndex == m_input.top())
        {
            if (action->m_action == ACTION_SHIFT)
                skJmp(lalrShift);
            if (action->m_action == ACTION_REDUCE)
                skJmp(lalrReduce);
            if (action->m_action == ACTION_GOTO)
                skJmp(lalrGoto);
            if (action->m_action == ACTION_ACCEPT)
                return MESSAGE_ACCEPT;
            skJmp(psrSyntax);
        }
        cntr++;
    }
    cntr = 0;

    skJmp(psrSyntax);

    // ----------------------------------------------------
    skLabel(lalrShift);

    m_stateIdx = action->m_value;
    item.m_state = 0;
    item.m_state = m_stateIdx;
    item.m_symbol = &m_symbolTable->m_symbols[action->m_symbolIndex];
    item.m_buffer = tmpBuf.c_str();
    item.m_primitive.pval = 0;
    m_stack.push(item);

    if (tokenAccepted(action->m_symbolIndex) == 0xFF)
        return MESSAGE_LEXICAL_ERROR;

    m_input.pop();

    skJmp(scnrRead);

    // ----------------------------------------------------
    skLabel(lalrReduce);

    m_input.push(m_ruleTable->m_rules[action->m_value].m_nonterminal);
    m_curReduction = m_ruleTable->m_rules[action->m_value].m_index;

    cntr = 0x0000;
    while (cntr < m_ruleTable->m_rules[action->m_value].m_count)
    {
        item = m_stack.top();
        item.m_symbol = &m_symbolTable->m_symbols[m_input.top()];
        m_stack.pop();
        ++cntr;
    }

    m_stateIdx = m_stack.top().m_state;
    skJmp(lalrAction);

    // ----------------------------------------------------
    skLabel(lalrGoto);

    m_stateIdx = action->m_value;
    item.m_state = m_stateIdx;
    item.m_symbol = &m_symbolTable->m_symbols[m_input.top()];

    m_stack.push(item);
    m_input.pop();

    if (execRule((SKuint8)m_curReduction) == 0xFF)
        return MESSAGE_INTERNAL_ERROR;

    if (m_input.empty())
        skJmp(scnrRead);

    skJmp(lalrAction);

    // ----------------------------------------------------
    skLabel(psrSyntax);
    return MESSAGE_SYNTAX_ERROR;
}

void skParser::stackTrace(void)
{
    int                          idx = 0;
    skStack<StackItem>::Iterator it = m_stack.iterator();
    while (it.hasMoreElements())
    {
        StackItem& lit = it.getNext();
        skPrintf("Stack Idx %3d: <Symbol %3d>: %s\n", idx, lit.m_symbol->m_index, lit.m_buffer.c_str());

        ++idx;
    }

    idx = 0;
    skStack<SKint16>::Iterator iit = m_input.iterator();
    while (iit.hasMoreElements())
    {
        SKint16 lit = iit.getNext();
        skPrintf("Input Idx %3d: %3d\n", idx, lit);
        ++idx;
    }
}
