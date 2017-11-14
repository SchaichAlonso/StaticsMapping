#include <Obj8/Parser/SyntaxError.hpp>

#include "Integer.hpp"


Obj8::Parameter::IntegerPrivate::IntegerPrivate (StringRef str, Parser::LexerContext *data, int flags)
: NumberPrivate (str, data, flags)
{
}



Obj8::Parameter::IntegerPrivate::IntegerPrivate (Numerator n)
: NumberPrivate (n, 1)
{
}



Obj8::Parameter::IntegerPrivate::IntegerPrivate (const IntegerPrivate &other)
: NumberPrivate (other)
{
}



Obj8::Parameter::IntegerPrivate::~IntegerPrivate ()
{
}



Obj8::Parser::WordPrivate *
Obj8::Parameter::IntegerPrivate::clone () const
{
  return (new IntegerPrivate (*this));
}



bool
Obj8::Parameter::IntegerPrivate::isValid () const
{
  return ((NumberPrivate::isValid()) and (isInt()));
}



Obj8::Parameter::Integer::Integer (Numerator n)
  : Number (new IntegerPrivate (n))
{
}



Obj8::Parameter::Integer::Integer (StringRef str, Parser::LexerContext *data)
  : Number (new IntegerPrivate (str, data))
{
  if (not isValid ()) {
    reject ();
  }
}



Obj8::Parameter::Integer::Integer (IntegerPrivate *dptr)
  : Number (dptr)
{
}



Obj8::Parameter::Integer::Integer (const Integer &other)
  : Number (other)
{
}



Obj8::Parameter::Integer::~Integer ()
{
}



Obj8::Parameter::Integer &
Obj8::Parameter::Integer::operator= (const Integer &other)
{
  Parameter::Number::operator= (other);
  
  return (*this);
}
