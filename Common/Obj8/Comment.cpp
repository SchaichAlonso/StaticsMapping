#include "Parser/SyntaxError.hpp"

#include "Comment.hpp"



Obj8::Comment::Comment ()
  : Record ()
{
}



Obj8::Comment::Comment (StringRef str, Parser::LexerContext *ctx, int flags)
  : Record (str, ctx, Parser::Word::ConsumeRemainingLine | flags)
{
  if (not qualified (m_identifier)) {
    throw (Parser::SyntaxError(ctx));
  }
}



Obj8::Comment::~Comment ()
{
}



bool
Obj8::Comment::qualified (Parser::Word w)
{
  StringRef id = w.value ();
  String::ConstIterator i, e;
  bool retval;
  
  i  = id.constBegin ();
  e  = id.constEnd ();
  
  if (i == e) {
    retval = false;
  } else {
    if (*i == '#') {
      retval = true;
    } else {
      retval = false;
    }
  }
  
  return (retval);
}



void
Obj8::Comment::accept (AbstractVisitor *, bool)
{
}


Obj8::RecordPointer
Obj8::Comment::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Comment (str, ctx)));
}



Obj8::String
Obj8::Comment::name () const
{
  Q_ASSERT (false);
  return (String ());
}
