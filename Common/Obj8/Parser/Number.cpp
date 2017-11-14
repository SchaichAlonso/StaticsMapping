#include "Number.hpp"
#include "SyntaxError.hpp"



Obj8::Parser::NumberPrivate::NumberPrivate (StringRef string, LexerContext *lc, int flags)
: WordPrivate (string, lc, flags & ~Word::EmptyWordAllowed)
, m_numerator (0)
, m_denominator (1)
, m_sign (0)
, m_integer_digits (0)
{
  const QChar *i, *e, *first;
  int ch, digit;
  
  i = m_value.constData ();
  e = i + m_value.length ();
  
  m_sign = (*i == '+')?  1 : 0;
  m_sign = (*i == '-')? -1 : m_sign;
  i = ((m_sign != 0)? 1 : 0) + i;
  
  first = i;
  
  for (int denom_growth=1; i != e; ++i) {
    
    ch = i->unicode ();
    switch (ch) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        digit         = ch - '0';
        m_numerator   = 10 * m_numerator + digit;
        m_denominator = m_denominator * denom_growth;
        
        m_integer_digits += (denom_growth == 1)? 1 : 0;
        
        break;
        
      case '.':
        if (denom_growth != 1) {
          throw (SyntaxError (m_lexer_context));
        }
        denom_growth = 10;
        break;
        
      default:
        throw (SyntaxError (m_lexer_context));
    }
  }
  
  if (m_sign == -1) {
     m_numerator = -m_numerator;
     m_sign = (m_numerator != 0)? 0 : m_sign;
  }
  
}



Obj8::Parser::NumberPrivate::NumberPrivate (Numerator n, Denominator d)
: WordPrivate ()
, m_numerator (n)
, m_denominator (d)
, m_sign (0)
, m_integer_digits (1)
{
}



Obj8::Parser::NumberPrivate::NumberPrivate (const NumberPrivate &other)
: WordPrivate (other)
, m_numerator (other.m_numerator)
, m_denominator (other.m_denominator)
, m_sign (other.m_sign)
, m_integer_digits (other.m_integer_digits)
{
}



Obj8::Parser::NumberPrivate::~NumberPrivate ()
{
}



Obj8::Parser::WordPrivate *
Obj8::Parser::NumberPrivate::clone () const
{
  return (new NumberPrivate (*this));
}



Obj8::Parser::NumberPrivate::Denominator
Obj8::Parser::NumberPrivate::denominator () const
{
  return (m_denominator);
}



void
Obj8::Parser::NumberPrivate::setDenominator (Denominator d)
{
  m_denominator = d;
}



Obj8::Parser::NumberPrivate::Numerator
Obj8::Parser::NumberPrivate::numerator () const
{
  return (m_numerator);
}


void
Obj8::Parser::NumberPrivate::setNumerator (Numerator n)
{
  m_numerator = n;
}



double
Obj8::Parser::NumberPrivate::toDouble () const
{
  return ((long double)m_numerator) / m_denominator;
}


int
Obj8::Parser::NumberPrivate::toInt () const
{
  return (m_numerator / m_denominator);
}



bool
Obj8::Parser::NumberPrivate::isInt () const
{
  return (m_denominator == 1);
}



bool
Obj8::Parser::NumberPrivate::isValid () const
{
  Denominator d, f;
  
  d = denominator ();
  
  if (d <= 0) {
    return (false);
  }
  
  f = 0;
  while ((d != 1) and (f == 0)) {
    f = d % 10;
    d = d / 10;
  }
  
  return (f == 0);
}



Obj8::String
Obj8::Parser::NumberPrivate::value () const
{
  String retval, integer;
  qlonglong prefix, suffix;
  bool negative;
  
  prefix = m_numerator / m_denominator;
  suffix = m_numerator % m_denominator;
  
  negative = (m_numerator < 0);
  prefix = negative? -prefix : prefix;
  suffix = negative? -suffix : suffix;
  
  if (negative) {
    retval.append ("-");
  } else {
    if (m_sign == 1) {
      retval.append ("+");
    } else if ((m_sign == -1) and (m_numerator == 0)) {
      retval.append ("-");
    }
  }
  
  if ((m_integer_digits != 0) or (prefix != 0)) {
    integer = QString::number (prefix);
  }
  
  if (integer.length() < m_integer_digits) {
    integer = integer.rightJustified (m_integer_digits, '0');
  }
  
  retval.append(integer);
  if (m_denominator != 1) {
    retval.append (".");
    
    int length = 0;
    for (Denominator copy=m_denominator; copy != 1; copy/=10) {
      ++length;
    }
    retval.append (QString::number(suffix).rightJustified(length, '0'));
  }
  return (retval);
}



void
Obj8::Parser::NumberPrivate::setValue (StringRef)
{
  /*
   * Don't call setValue on Numbers.
   */
  Q_ASSERT (false);
}



void
Obj8::Parser::NumberPrivate::toCommonDenominator (
    Numerator   *an, Numerator   *bn,
    Denominator *ad, Denominator *bd
)
{
  while (*ad < *bd) {
    *an *= 10;
    *ad *= 10;
  }
  while (*bd < *ad) {
    *bn *= 10;
    *bd *= 10;
  }
  Q_ASSERT (*ad == *bd);
}


void
Obj8::Parser::NumberPrivate::toDenominator (Denominator d)
{
  Q_ASSERT (d > 0);
  
  while (m_denominator < d) {
    m_denominator *= 10;
    m_numerator *= 10;
  }
  
  while (d < m_denominator) {
    m_denominator /= 10;
    m_numerator /= 10;
  }
  
  Q_ASSERT (d == m_denominator);
}



void
Obj8::Parser::NumberPrivate::addsub (const NumberPrivate *rval, bool add)
{
  Numerator   an = this->numerator ();
  Numerator   bn = rval->numerator ();
  Denominator ad = this->denominator ();
  Denominator bd = rval->denominator ();
  
  toCommonDenominator (&an, &bn, &ad, &bd);
  
  Q_ASSERT(ad == bd);
  
  if (add)
    an += bn;
  else
    an -= bn;
  
  setNumerator (an);
  setDenominator (ad);
}



bool
Obj8::Parser::NumberPrivate::numericEqualTo (const NumberPrivate *rval) const
{
  Numerator   an = this->numerator ();
  Numerator   bn = rval->numerator ();
  Denominator ad = this->denominator ();
  Denominator bd = rval->denominator ();
  
  toCommonDenominator (&an, &bn, &ad, &bd);
  
  return (an == bn);
}



Obj8::Parser::Number::Number (Numerator n, Denominator d)
  : Word (new NumberPrivate (n, d))
{
}



Obj8::Parser::Number::Number (StringRef str, LexerContext *data, int flags)
  : Word (new NumberPrivate (str, data, flags))
{
}



Obj8::Parser::Number::Number (NumberPrivate *dptr)
  : Word (dptr)
{
}



Obj8::Parser::Number::Number (const Number &other)
  : Word (other)
{
}



Obj8::Parser::Number::~Number ()
{
}



Obj8::Parser::Number &
Obj8::Parser::Number::operator= (const Number &other)
{
  Word::operator= (other);
  
  return (*this);
}



double
Obj8::Parser::Number::toDouble () const
{
  return (context()->toDouble());
}



int
Obj8::Parser::Number::toInt () const
{
  return (context()->toInt());
}



bool
Obj8::Parser::Number::isInt () const
{
  return (context()->isInt());
}



Obj8::Parser::Number
Obj8::Parser::Number::operator+ (Number other) const
{
  Number retval(*this);
  retval.detach();
  retval.context()->addsub(other.context(), true);
  return (retval);
}



Obj8::Parser::Number
Obj8::Parser::Number::operator- (Number other) const
{
  Number retval(*this);
  retval.detach();
  retval.context()->addsub(other.context(), false);
  return (retval);
}



Obj8::Parser::Number
Obj8::Parser::Number::operator- () const
{
  Number retval;
  retval.context()->addsub(context(), false);
  return (retval);
}



bool
Obj8::Parser::Number::operator == (const Number &other) const
{
  const NumberPrivate *a = this->context ();
  const NumberPrivate *b = other.context ();
  
  return (a->numericEqualTo(b));
}



bool
Obj8::Parser::Number::isValid () const
{
  return (context()->isValid());
}



Obj8::Parser::Number::Denominator
Obj8::Parser::Number::denominator () const
{
  return (context()->denominator());
}



Obj8::Parser::Number::Numerator
Obj8::Parser::Number::numerator () const
{
  return (context()->numerator());
}



void
Obj8::Parser::Number::setDenominator (Denominator d)
{
  detach();
  context()->setDenominator(d);
}



void
Obj8::Parser::Number::setNumerator (Numerator n)
{
  detach();
  context()->setNumerator(n);
}



void
Obj8::Parser::Number::toDenominator (Denominator d)
{
  detach ();
  context()->toDenominator(d);
}



Obj8::Parser::NumberPrivate *
Obj8::Parser::Number::context ()
{
  NumberPrivate *ctx;
  
  ctx = dynamic_cast<NumberPrivate *> (m_dptr);
  
  Q_CHECK_PTR (ctx);
  
  return (ctx);
}



const Obj8::Parser::NumberPrivate *
Obj8::Parser::Number::context () const
{
  const NumberPrivate *ctx;
  
  ctx = dynamic_cast<const NumberPrivate *> (m_dptr);
  
  Q_CHECK_PTR (ctx);
  
  return (ctx);
}

