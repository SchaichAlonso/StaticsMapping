#include "Index.hpp"



Obj8::Data::Index::Index ()
  : AbstractIndex ()
{
}



Obj8::Data::Index::Index (StringRef string, Parser::LexerContext *ctx)
  : AbstractIndex (string, ctx)
{
  readIndex (string, ctx);
}



Obj8::Data::Index::~Index()
{
}



Obj8::RecordPointer
Obj8::Data::Index::instantiate (StringRef string, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Index (string, ctx)));
}



void
Obj8::Data::Index::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::String
Obj8::Data::Index::name () const
{
  return ("IDX");
}
