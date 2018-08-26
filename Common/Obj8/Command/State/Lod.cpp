#include "Lod.hpp"



Obj8::Command::State::Lod::Lod ()
  : Record ()
  , m_near ()
  , m_far ()
{
}



Obj8::Command::State::Lod::Lod (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_near (str, ctx)
  , m_far (str, ctx)
{
}



Obj8::Command::State::Lod::~Lod ()
{
}



void
Obj8::Command::State::Lod::accept (AbstractVisitor *visitor, bool)
{
  visitor->visit (this);
}



Obj8::RecordPointer
Obj8::Command::State::Lod::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new Lod (str, ctx)));
}



Obj8::String
Obj8::Command::State::Lod::name () const
{
  return ("ATTR_LOD");
}



Obj8::String
Obj8::Command::State::Lod::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_near.toString ();
  retval += m_far.toString ();
  
  return (retval);
}



int
Obj8::Command::State::Lod::nearDistance() const
{
  return (m_near.toInt ());
}



int
Obj8::Command::State::Lod::farDistance() const
{
  return (m_far.toInt ());
}
