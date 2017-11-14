#include "HardDeck.hpp"



Obj8::Command::State::HardDeck::HardDeck ()
  : AbstractHard ()
{
}



Obj8::Command::State::HardDeck::HardDeck (StringRef str, Parser::LexerContext *ctx)
  : AbstractHard (str, ctx)
{
}



Obj8::Command::State::HardDeck::~HardDeck ()
{
}



Obj8::RecordPointer
Obj8::Command::State::HardDeck::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new HardDeck (str, ctx)));
}



Obj8::String
Obj8::Command::State::HardDeck::name () const
{
  return ("ATTR_hard_deck");
}
