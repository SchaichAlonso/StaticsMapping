#include "LexerContext.hpp"
#include "SyntaxError.hpp"



Obj8::Parser::LexerContext::LexerContext ()
: m_line (1)
, m_col (1)
, m_index (0)
, m_eol (IBM)
{
}



Obj8::Parser::LexerContext::LexerContext (const LexerContext &other)
: m_line (other.m_line)
, m_col (other.m_col)
, m_index (other.m_index)
, m_eol (other.m_eol)
{
}



Obj8::Parser::LexerContext::LexerContext (const LexerContext *other)
{
  if (other != Q_NULLPTR) {
    *this = *other;
  } else {
    *this = LexerContext ();
  }
}



Obj8::Parser::LexerContext::~LexerContext ()
{
}



Obj8::Parser::LexerContext &
Obj8::Parser::LexerContext::operator= (const LexerContext &other)
{
  m_line  = other.m_line;
  m_col   = other.m_col;
  m_index = other.m_index;
  m_eol   = other.m_eol;
  
  return (*this);
}



bool
Obj8::Parser::LexerContext::atEndOf (StringRef str) const
{
  int ch;
  
  ch = peek (str);
  
  return (ch == EndOfFile);
}



int
Obj8::Parser::LexerContext::peek (StringRef str) const
{
  LexerContext copy (this);
  int retval;
  
  retval = read (&copy, str);
  
  return (retval);
}



int
Obj8::Parser::LexerContext::read (StringRef str)
{
  int retval;
  
  retval = read (this, str);
  
  return (retval);
}




Obj8::String
Obj8::Parser::LexerContext::lineEnding (int ch) const
{
  char retval[3];
  
  switch (ch) {
    case ShortEndOfLine:
      retval[0] = (m_eol == Apple)? '\r' : '\n';
      retval[1] = '\0';
      break;
    case LongEndOfLine:
      retval[0] = '\r';
      retval[1] = '\n';
      break;
    default:
      retval[0] = ch;
      retval[1] = '\0';
  } 
  retval[2] = '\0';
  
  return (retval);
}




Obj8::Parser::LexerContext::LineEnding
Obj8::Parser::LexerContext::lineEnding () const
{
  return (m_eol);
}



void
Obj8::Parser::LexerContext::setLineEnding (LineEnding eol)
{
  m_eol = eol;
}



int
Obj8::Parser::LexerContext::column () const
{
  return (m_col);
}



int
Obj8::Parser::LexerContext::line () const
{
  return (m_line);
}



int
Obj8::Parser::LexerContext::read (LexerContext *ctx, StringRef str)
{
  String::ConstIterator i, e;
  int cur, next;
  
  i = str.constBegin ();
  e = str.constEnd ();
  
  i += ctx->m_index;
  
  if (i >= e) {
    return (EndOfFile);
  }
  
  cur = i->unicode ();
  
  if ((cur & 0x7f) != cur) {
    throw (SyntaxError (ctx));
  }
  
  switch (cur) {
  case '\r':
    if (ctx->m_eol == Apple) {
      cur = ShortEndOfLine;
    } else {
      if (i+1 != e) {
        next = (i+1)->unicode ();
        if (next == '\n') {
          cur = LongEndOfLine;
          ctx->m_index += 1;
        }
      }
    }
    break;
  
  case '\n':
    if (ctx->m_eol == IBM) {
      cur = ShortEndOfLine;
    }
    break;
  
  default:
    ;
  }
  
  ctx->m_index += 1;
  
  ctx->m_col   += 1;
  ctx->m_col    = ((cur & EndOfLineMask) == cur)? 1 : ctx->m_col;
  ctx->m_line  += ((cur & EndOfLineMask) == cur)? 1 : 0;
  
  return (cur);
}
