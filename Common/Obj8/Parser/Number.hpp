#pragma once

#include <QtGlobal>

#include "Parser.hpp"
#include "Word.hpp"

namespace Obj8
{
  namespace Parser
  {
    struct NumberPrivate;
  
    struct Number : Word
    {
      typedef qlonglong Numerator;
      typedef qlonglong Denominator;
    
      Number (Numerator=0, Denominator=1);
      Number (NumberPrivate *);
      Number (const Number &);
      Number (StringRef, LexerContext * =Q_NULLPTR, int =0);
      virtual ~Number ();
      
      Number &operator= (const Number &);
      
      double toDouble () const;
      int    toInt () const;
      bool   isInt () const;
      bool   isValid () const;
      
      Number operator+ (Number) const;
      Number operator- (Number) const;
      Number operator- () const;
      bool   operator== (const Number &) const;
      
      Denominator denominator () const;
      void setDenominator (Denominator);
      Numerator numerator() const;
      void setNumerator (Numerator);
      
      void toDenominator (Denominator);
      
    protected:
            NumberPrivate *context ();
      const NumberPrivate *context () const;
    };
    
    struct NumberPrivate : WordPrivate
    {
      typedef Number::Numerator Numerator;
      typedef Number::Denominator Denominator;
      
      
      NumberPrivate (StringRef, LexerContext * =Q_NULLPTR, int =0);
      NumberPrivate (Numerator=0, Denominator=1);
      NumberPrivate (const NumberPrivate &);
      virtual ~NumberPrivate ();
      
      virtual WordPrivate *clone () const Q_DECL_OVERRIDE;
      
      virtual Denominator denominator () const;
      virtual void        setDenominator (Denominator);
      virtual Numerator   numerator() const;
      virtual void        setNumerator (Numerator);
      
      virtual double  toDouble () const;
      virtual int     toInt () const;
      virtual bool    isInt () const;
      virtual bool    isValid () const;
      
      virtual String  value () const Q_DECL_OVERRIDE;
      virtual void    setValue (StringRef) Q_DECL_OVERRIDE;
      
      static void toCommonDenominator (Numerator *, Numerator *, Denominator *, Denominator *);
      void toDenominator (Denominator);
      
      void addsub (const NumberPrivate *, bool);
      bool numericEqualTo (const NumberPrivate *) const;
      
    protected:
      Numerator   m_numerator;
      Denominator m_denominator;
      int         m_sign;
      int         m_integer_digits;
    };
  }
}
