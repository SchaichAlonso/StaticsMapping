#include "Number.hpp"



Obj8::Parameter::NumberPrivate::NumberPrivate (StringRef str, Parser::LexerContext *data, int flags)
: Parser::NumberPrivate (str, data, Parser::Word::PrecedingLinebreakNotAllowed | flags)
{
}



Obj8::Parameter::NumberPrivate::NumberPrivate (Numerator n, Denominator d)
: Parser::NumberPrivate (n, d)
{
}



Obj8::Parameter::NumberPrivate::NumberPrivate (const NumberPrivate &other)
: Parser::NumberPrivate (other)
{
}



Obj8::Parameter::NumberPrivate::~NumberPrivate ()
{
}



Obj8::Parser::WordPrivate *
Obj8::Parameter::NumberPrivate::clone () const
{
  return (new NumberPrivate (*this));
}





Obj8::Parameter::Number::Number (Numerator n, Denominator d)
  : Parser::Number (new NumberPrivate (n, d))
{
}



Obj8::Parameter::Number::Number (StringRef str, Parser::LexerContext *data)
  : Parser::Number (new NumberPrivate (str, data))
{
}



Obj8::Parameter::Number::Number (NumberPrivate *dptr)
  : Parser::Number (dptr)
{
}



Obj8::Parameter::Number::Number (const Number &other)
  : Parser::Number (other)
{
}



Obj8::Parameter::Number::~Number ()
{
}



Obj8::Parameter::Number &
Obj8::Parameter::Number::operator= (const Number &other)
{
  Parser::Number::operator= (other);
  
  return (*this);
}



bool
Obj8::Parameter::Number::hasUnitLength (const Number &x1, const Number &x2, const Number &x3)
{
  double a = x1.toDouble();
  double b = x2.toDouble();
  double c = x3.toDouble();
  
  float mag = a*a + b*b + c*c;
  return (qFuzzyCompare (mag, 1));
}
