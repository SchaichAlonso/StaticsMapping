#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Number.hpp>
#include <Obj8/Parser/Number.hpp>



namespace Obj8
{
  namespace Parameter
  {
    struct RatioPrivate : NumberPrivate
    {
      RatioPrivate (StringRef, Parser::LexerContext * =Q_NULLPTR, int =0);
      RatioPrivate (Numerator =0, Denominator =0);
      RatioPrivate (const NumberPrivate &);
      virtual ~RatioPrivate ();
      
      virtual WordPrivate *clone () const Q_DECL_OVERRIDE;
      virtual bool         isValid () const Q_DECL_OVERRIDE;
    };
    
    
    
    struct Ratio : Number
    {
      Ratio (Numerator=0, Denominator=0);
      Ratio (RatioPrivate *);
      Ratio (const Ratio &);
      Ratio (StringRef, Parser::LexerContext * =Q_NULLPTR);
      virtual ~Ratio ();
    
      Ratio &operator= (const Ratio &);
    };
  }
}
