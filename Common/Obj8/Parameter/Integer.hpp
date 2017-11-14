#pragma once

#include <QtGlobal>

#include <Obj8/Parameter/Number.hpp>
#include <Obj8/Parser/Number.hpp>



namespace Obj8
{
  namespace Parameter
  {
    struct IntegerPrivate : NumberPrivate
    {
      IntegerPrivate (StringRef, Parser::LexerContext * =Q_NULLPTR, int =0);
      IntegerPrivate (Numerator=0);
      IntegerPrivate (const IntegerPrivate &);
      virtual ~IntegerPrivate ();
      
      virtual WordPrivate *clone () const Q_DECL_OVERRIDE;
      virtual bool         isValid () const Q_DECL_OVERRIDE;
    };
    
    
    struct Integer : Number
    {
      Integer (Numerator=0);
      Integer (IntegerPrivate *);
      Integer (const Integer &);
      Integer (StringRef, Parser::LexerContext * =Q_NULLPTR);
      virtual ~Integer ();
    
      Integer &operator= (const Integer &);
    };
  }
}
