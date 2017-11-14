#pragma once

#include <QtGlobal>

#include <Obj8/Parser/Number.hpp>

namespace Obj8
{
  namespace Parameter
  {
    struct NumberPrivate : Parser::NumberPrivate
    {
      NumberPrivate (StringRef, Parser::LexerContext * =Q_NULLPTR, int =0);
      NumberPrivate (Numerator =0, Denominator =0);
      NumberPrivate (const NumberPrivate &);
      virtual ~NumberPrivate ();
      
      virtual WordPrivate *clone () const Q_DECL_OVERRIDE;
    };
    
    
    struct Number : Parser::Number
    {
      Number (Numerator=0, Denominator=0);
      Number (NumberPrivate *);
      Number (const Number &);
      Number (StringRef, Parser::LexerContext * =Q_NULLPTR);
      virtual ~Number ();
    
      Number &operator= (const Number &);
      
      
      static bool hasUnitLength (const Number &, const Number &, const Number &);
    };
  }
}
