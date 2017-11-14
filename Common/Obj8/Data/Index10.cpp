#include "Index10.hpp"



Obj8::Data::Index10::Index10 ()
  : AbstractIndex ()
{
}



Obj8::Data::Index10::Index10 (StringRef string, Parser::LexerContext *ctx)
  : AbstractIndex (string, ctx)
{
  for (int i=0; i!=10; ++i) {
    readIndex (string, ctx);
  }
}



Obj8::Data::Index10::~Index10 ()
{
}



Obj8::RecordPointer
Obj8::Data::Index10::instantiate (StringRef string, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Index10 (string, ctx)));
}



void
Obj8::Data::Index10::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::String
Obj8::Data::Index10::name () const
{
  return ("IDX10");
}
