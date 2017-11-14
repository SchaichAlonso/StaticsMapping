#include "AbstractIndex.hpp"



Obj8::AbstractIndex::AbstractIndex ()
  : Record ()
  , m_values ()
{
}



Obj8::AbstractIndex::AbstractIndex (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_values ()
{
}



Obj8::AbstractIndex::~AbstractIndex ()
{
}


int
Obj8::AbstractIndex::index (int index) const
{
  Indices::ConstIterator i;
  
  Q_ASSERT (index < m_values.size());
  
  for (i = m_values.constBegin(); index!=0; --index, ++i);
  
  return (i->toInt());
}



Obj8::AbstractIndex::Indices
Obj8::AbstractIndex::indices () const
{
  return (m_values);
}



Obj8::String
Obj8::AbstractIndex::toString () const
{
  String retval;
  
  retval = Record::toString ();
  Q_FOREACH (Parameter::Integer const &param, m_values) {
    retval += param.toString ();
  }
  
  return (retval);
}



void
Obj8::AbstractIndex::readIndex (StringRef str, Parser::LexerContext *ctx)
{
  m_values.append (Parameter::Integer (str, ctx));
}
