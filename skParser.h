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
#ifndef _skParser_h_
#define _skParser_h_

#include "skAllocator.h"
#include "skFixedString.h"
#include "skStack.h"
#include "skString.h"

class skStream;

class skParser : public skAllocObject
{
public:
    typedef SKint16 Index;
    typedef SKint16 Token;

    union StackPrimitive {
        long   ival;
        double dval;
        char*  sval;
        void*  pval;
    };

    struct Symbol
    {
        Index       m_index;
        Index       m_kind;
        const char* m_symbol;
    };

    struct SymbolTable
    {
        Index   m_count;
        Symbol* m_symbols;
    };

    struct Rule
    {
        Index   m_index;
        Index   m_nonterminal;
        Index   m_count;
        Symbol* m_symbols;
    };

    struct RuleTable
    {
        Index m_count;
        Rule* m_rules;
    };

    struct DFAEdge
    {
        Index m_char;
        Index m_target;
    };

    struct DFAState
    {

        Index    m_accept;
        Index    m_edgeCount;
        DFAEdge* m_edges;
    };

    struct DFATable
    {

        Index     m_count;
        Index     m_initial;
        DFAState* m_states;
    };

    struct LALRAction
    {
        Index m_symbolIndex;
        Index m_action;
        Index m_value;
    };

    struct LALRState
    {
        Index       m_count;
        LALRAction* m_action;
    };

    struct LALRTable
    {

        Index      m_count;
        Index      m_initial;
        LALRState* m_states;
    };

    struct CharacterSet
    {
        Index  m_count;
        Index* m_chars;
    };

    struct CharacterSetTable
    {
        SKint16             m_count;
        SKint16             m_isExpanded;
        const CharacterSet* m_chars;
    };

    struct StackItem
    {
        Symbol*        m_symbol;
        SKint16        m_state;
        StackPrimitive m_primitive;
        skString       m_buffer;
        void*          m_ptr;
    };

    enum Action
    {
        ACTION_SHIFT = 1,
        ACTION_REDUCE,
        ACTION_GOTO,
        ACTION_ACCEPT
    };

    enum SymbolKind
    {
        SM_NONTERMINAL = 0,
        SM_TERMINAL,
        SM_WHITESPACE,
        SM_EOF,
        SM_COMMENT_START,
        SM_COMMENT_END,
        SM_COMMENT_LINE,
        SM_ERROR
    };

    enum ParseResult
    {
        PR_SHIFT = 1,
        PR_REDUCE,
        PR_REDUCE_TRIMMED,
        PR_ACCEPT,
        PR_SYNTAX_ERROR,
        PR_INTERNAL_ERROR
    };

    enum Message
    {
        MESSAGE_TOKEN_READ = 1,
        MESSAGE_REDUCTION,
        MESSAGE_ACCEPT,
        MESSAGE_NOT_LOADED_ERROR,
        MESSAGE_LEXICAL_ERROR,
        MESSAGE_SYNTAX_ERROR,
        MESSAGE_UNTERMINATED_ERROR,
        MESSAGE_UNMATCHED_ERROR,
        MESSAGE_INTERNAL_ERROR
    };

public:
    skParser();
    ~skParser();

    void initialize(SKuint32 stackSize, skStream* in);

protected:
    virtual SKint16 execRule(SKuint8 rule) = 0;
    virtual SKint16 tokenAccepted(SKint16 act) = 0;
    virtual SKint32 handleBlockComment(char* inp, SKint32 from, SKint32 len, SKint32& out, skString* bo)
    {
        return -1;
    }

    Message parseImpl(void);

    virtual void stackTrace(void);

    skStack<Token>     m_input;
    skStack<StackItem> m_stack;
    skStream*          m_stream;

    SKint16            m_start;
    SymbolTable*       m_symbolTable;
    CharacterSetTable* m_charsets;
    RuleTable*         m_ruleTable;
    DFATable*          m_dfa;
    LALRTable*         m_lalr;
    SKbyte*            m_buffer;
    SKuint32           m_len;
    SKuint32           m_stateIdx;
    SKuint32           m_cur;
    SKuint16           m_curReduction;
};

#endif  //_skParser_h_
