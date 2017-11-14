#include "Ratio.hpp"

Obj8::Parameter::RatioPrivate::RatioPrivate (StringRef str, Parser::LexerContext *data, int flags)
: NumberPrivate (str, data, flags)
{
}



Obj8::Parameter::RatioPrivate::RatioPrivate (Numerator n, Denominator d)
: NumberPrivate (n, d)
{
}



Obj8::Parameter::RatioPrivate::RatioPrivate (const NumberPrivate &other)
: NumberPrivate (other)
{
}



Obj8::Parameter::RatioPrivate::~RatioPrivate ()
{
}



Obj8::Parser::WordPrivate *
Obj8::Parameter::RatioPrivate::clone () const
{
  return (new RatioPrivate (*this));
}



bool
Obj8::Parameter::RatioPrivate::isValid () const
{
  if (NumberPrivate::isValid ()) {
    return ((0 <= m_numerator) and (m_numerator <= m_denominator));
  }
  
  return (false);
}










Obj8::Parameter::Ratio::Ratio (Numerator n, Denominator d)
  : Number (new RatioPrivate (n, d))
{
}



Obj8::Parameter::Ratio::Ratio (StringRef str, Parser::LexerContext *ctx)
  : Number (new RatioPrivate (str, ctx))
{
  if (not isValid ()) {
    reject ();
  }
}



Obj8::Parameter::Ratio::Ratio (RatioPrivate *dptr)
  : Number (dptr)
{
}



Obj8::Parameter::Ratio::Ratio (const Ratio &other)
  : Number (other)
{
}



Obj8::Parameter::Ratio::~Ratio ()
{
}



Obj8::Parameter::Ratio &
Obj8::Parameter::Ratio::operator= (const Ratio &other)
{
  Number::operator= (other);
  
  return (*this);
}
