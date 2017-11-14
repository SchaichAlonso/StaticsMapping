#pragma once

#include <Obj8/Obj8.hpp>
#include <Obj8/Parser/LexerContext.hpp>
#include <Obj8/Parser/SyntaxError.hpp>



namespace Obj8
{
  namespace Test
  {
    struct ParserHelper
    {
      template <typename Type>
      bool 
      reproduces (Obj8::String want, Obj8::Parser::LexerContext ctx, int flags=0) const
      {
        Type n (want, &ctx, flags);
        Obj8::String have = n.toString ();
        
        return (have == want);
      }
      
      
      template <typename Type>
      bool 
      reproduces (Obj8::String want) const
      {
        Obj8::Parser::LexerContext ctx;
        Type n (want, &ctx);
        Obj8::String have = n.toString ();
        
        return (have == want);
      }


      template <typename Type>
      bool 
      reproduces (Obj8::String want, int flags) const
      {
        return (reproduces<Type>(want, Obj8::Parser::LexerContext(), flags));
      }



      template <typename Type>
      bool
      accepts (Obj8::String want, Obj8::Parser::LexerContext::LineEnding le) const
      {
        bool accepted;
        
        try {
          Obj8::Parser::LexerContext ctx;
          ctx.setLineEnding (le);
          accepted = reproduces<Type> (want, ctx);
        } catch (const Obj8::Parser::SyntaxError &) {
          accepted = false;
        } 
        
        return (accepted);
      }



      template <typename Type>
      bool
      accepts (Obj8::String want, int flags) const
      {
        bool accepted;
        
        try {
          accepted = reproduces<Type> (want, flags);
        } catch (const Obj8::Parser::SyntaxError &) {
          accepted = false;
        } 
        
        return (accepted);
      }



      template <typename Type>
      bool
      accepts (Obj8::String want) const
      {
        bool accepted;
        
        try {
          accepted = reproduces<Type> (want);
        } catch (const Obj8::Parser::SyntaxError &) {
          accepted = false;
        } 
        
        return (accepted);
      }
    };
  }
}
