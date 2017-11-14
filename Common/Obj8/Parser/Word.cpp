#include "SyntaxError.hpp"
#include "Word.hpp"



Obj8::Parser::WordPrivate::WordPrivate ()
: m_references (0)
, m_indentation ()
, m_value ()
, m_lexer_context ()
{
}



Obj8::Parser::WordPrivate::WordPrivate (StringRef string, LexerContext *ctx, int flags)
: m_references (0)
, m_indentation ()
, m_value ()
, m_lexer_context (ctx)
{
  LexerContext lookahead;
  QChar ch;
  int  cp;
  bool reading_indentation, end_of_line_seen;
  
  reading_indentation = true;
  end_of_line_seen = false;
  
  for (lookahead=m_lexer_context; ; m_lexer_context=lookahead) {
    cp = lookahead.read (string);
    ch = cp;
    
    switch (cp) {
    case LexerContext::EndOfFile:
      break;
      
    case ' ':
    case '\t':
      if (reading_indentation) {
        m_indentation += ch;
        continue;
      }
      if (flags & Word::ConsumeRemainingLine) {
        m_value += ch;
        continue;
      }
      break;
    
    case LexerContext::ShortEndOfLine:
    case LexerContext::LongEndOfLine:
      if (reading_indentation) {
        if (flags & Word::PrecedingLinebreakNotAllowed) {
          throw (SyntaxError (m_lexer_context));
        }
        end_of_line_seen = true;
        m_indentation += ch;
        continue;
      }
      break;
    
    default:
      if (flags & Word::PrecedingLinebreakRequired) {
        if (not end_of_line_seen) {
          throw (SyntaxError (m_lexer_context));
        }
      }
      reading_indentation = false;
      m_value += ch;
      continue;
    }
    break;
  }
  
  if (m_value.isEmpty() and not (flags & Word::EmptyWordAllowed)) {
    throw (SyntaxError (m_lexer_context));
  }
  
  if (ctx != Q_NULLPTR) {
    *ctx = m_lexer_context;
  }
}



Obj8::Parser::WordPrivate::WordPrivate (WordPrivate const &other)
: m_references (0)
, m_indentation (other.m_indentation)
, m_value (other.m_value)
, m_lexer_context (other.m_lexer_context)
{
}




Obj8::Parser::WordPrivate::~WordPrivate ()
{
  Q_ASSERT (m_references == 0);
}



Obj8::Parser::WordPrivate *
Obj8::Parser::WordPrivate::clone () const
{
  return (new WordPrivate (*this));
}



Obj8::String
Obj8::Parser::WordPrivate::indentation () const
{
  return (m_indentation);
}


void
Obj8::Parser::WordPrivate::setIndentation (StringRef indentation)
{
  m_indentation = indentation;
}



Obj8::String
Obj8::Parser::WordPrivate::value () const
{
  return (m_value);
}



void
Obj8::Parser::WordPrivate::setValue (StringRef value)
{
  m_value = value;
}



void
Obj8::Parser::WordPrivate::append (const WordPrivate *other)
{
  setValue (value() + other->indentation() + other->value());
  m_lexer_context = other->m_lexer_context;
}



bool
Obj8::Parser::WordPrivate::isEmpty () const
{
#if 0
  bool i = m_indentation.isEmpty ();
  bool v = m_value.isEmpty ();
  return (i and v);
#else 
  return (m_value.isEmpty ());
#endif
}



Obj8::String
Obj8::Parser::WordPrivate::toString () const
{
  return (restoreLineEndings (indentation() + value()));
}



Obj8::String
Obj8::Parser::WordPrivate::restoreLineEndings (StringRef str) const
{
  String retval;
  String::ConstIterator i, e;
  
  for (i=str.constBegin(), e=str.constEnd(); i!=e; ++i) {
    retval += m_lexer_context.lineEnding (i->unicode ());
  }
  
  return (retval);
}



Obj8::Parser::LexerContext
Obj8::Parser::WordPrivate::lexerContext () const
{
  return (m_lexer_context);
}



Obj8::Parser::Word::Word ()
  : m_dptr (new WordPrivate ())
{
  ++m_dptr->m_references;
}



Obj8::Parser::Word::Word (WordPrivate *dptr)
  : m_dptr (dptr)
{
  ++m_dptr->m_references;
}



Obj8::Parser::Word::Word (const Word &other)
  : m_dptr (other.m_dptr)
{
  ++m_dptr->m_references;
}




Obj8::Parser::Word::Word (StringRef str, LexerContext *ctx, int flags)
  : m_dptr (new WordPrivate (str, ctx, flags))
{
  ++m_dptr->m_references;
}



Obj8::Parser::Word::~Word ()
{
  Q_CHECK_PTR (m_dptr);
  
  if (--m_dptr->m_references == 0) {
    delete (m_dptr);
  }
  
  m_dptr = Q_NULLPTR;
}



Obj8::Parser::Word &
Obj8::Parser::Word::operator= (const Word& other)
{
  WordPrivate *delete_this;
  
  Q_CHECK_PTR (m_dptr);
  Q_CHECK_PTR (other.m_dptr);
  
  ++other.m_dptr->m_references;
  
  delete_this = (--m_dptr->m_references == 0) ? m_dptr : Q_NULLPTR;
  m_dptr = other.m_dptr;
  
  delete (delete_this);
  
  return (*this);
}



void
Obj8::Parser::Word::detach ()
{
  WordPrivate *clone;
  
  Q_CHECK_PTR (m_dptr);
  
  if (m_dptr->m_references != 1) {
    clone = m_dptr->clone ();
    --m_dptr->m_references;
    m_dptr = clone;
    m_dptr->m_references = 1;
  }
}



Obj8::Parser::Word
Obj8::Parser::Word::read (StringRef str, LexerContext *ctx, int flags)
{
  return (Word (str, ctx, flags));
}



Obj8::Parser::Word
Obj8::Parser::Word::peek (StringRef str, const LexerContext *ctx, int flags)
{
  LexerContext copy (ctx);
  return (Word (str, &copy, flags));
}



Obj8::String
Obj8::Parser::Word::toString () const
{
  Q_CHECK_PTR (m_dptr);
  return (m_dptr->toString());
}



bool
Obj8::Parser::Word::empty () const
{
  Q_CHECK_PTR (m_dptr);
  
  return (m_dptr->isEmpty ());
}



void
Obj8::Parser::Word::reject () const
{
  Q_CHECK_PTR (m_dptr);
  throw (SyntaxError (m_dptr->lexerContext ()));
}



Obj8::Parser::LexerContext
Obj8::Parser::Word::lexerContext () const
{
  Q_CHECK_PTR (m_dptr);
  return (m_dptr->lexerContext ());
}



Obj8::String
Obj8::Parser::Word::indentation () const
{
  Q_CHECK_PTR (m_dptr);
  return (m_dptr->indentation());
}



void
Obj8::Parser::Word::setIndentation (StringRef indentation)
{
  detach ();
  m_dptr->setIndentation(indentation);
}




Obj8::String
Obj8::Parser::Word::value () const
{
  Q_CHECK_PTR (m_dptr);
  return (m_dptr->value ());
}



void
Obj8::Parser::Word::setValue (StringRef value)
{
  detach ();
  m_dptr->setValue (value);
}



Obj8::Parser::Word &
Obj8::Parser::Word::append (const Word &other)
{
  detach ();
  m_dptr->append (other.m_dptr);
  
  return (*this);
}



Obj8::String
Obj8::Parser::Word::restoreLineEndings (StringRef str) const
{
  Q_CHECK_PTR (m_dptr);
  return (m_dptr->restoreLineEndings (str));
}
