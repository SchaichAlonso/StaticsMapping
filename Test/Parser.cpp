#include "Parser.hpp"

typedef Obj8::Parser::Number Number;
typedef Obj8::Parser::Word Word;



void
Obj8::Test::Parser::reprocudesNumbers ()
{
  for (int i=0; i!=10000; ++i) {
    QVERIFY (reproduces<Number> (QString::asprintf ("%d.%03d", i%10, i/10)));
  }
  
  QVERIFY(reproduces<Number> ("123.0"));
  QVERIFY(reproduces<Number> ("01.0"));
  
  QVERIFY(reproduces<Number> ("0.00"));
  QVERIFY(reproduces<Number> ("  -7.6"));
  QVERIFY(reproduces<Number> ("  1.234"));
  QVERIFY(reproduces<Number> (" \t -1.234"));
  
  QVERIFY(reproduces<Number> ("-0"));
  QVERIFY(reproduces<Number> ("+0"));
  QVERIFY(reproduces<Number> ("-1"));
  QVERIFY(reproduces<Number> ("+2"));
  
  QVERIFY(reproduces<Number> (".123"));
  QVERIFY(reproduces<Number> ("+.123"));
  QVERIFY(reproduces<Number> ("-.123"));
}


void
Obj8::Test::Parser::poorlyFormatedNumbers ()
{
  QVERIFY (not accepts<Number> (""));
  QVERIFY (not accepts<Number> ("  "));
  QVERIFY (not accepts<Number> ("1.2.3"));
  QVERIFY (not accepts<Number> ("1a2"));
}


void
Obj8::Test::Parser::reproducesWords ()
{
  QVERIFY (reproduces<Word> (" Hello"));
  QVERIFY (reproduces<Word> ("\tWorld"));
  QVERIFY (reproduces<Word> ("\nWorld"));
}



void
Obj8::Test::Parser::whitespace ()
{
  QVERIFY (not accepts<Word> (" \t\n"));
  QVERIFY (    accepts<Word> (" \t\n", Word::EmptyWordAllowed));
  
  QVERIFY (    accepts<Word> (" \t\nx"));
  QVERIFY (not accepts<Word> (" \t\nx", Word::PrecedingLinebreakNotAllowed));
  QVERIFY (    accepts<Word> (" \t\nx", Word::PrecedingLinebreakRequired));
  
  QVERIFY (    accepts<Word> (" \t\nx", Word::ConsumeRemainingLine));
  QVERIFY (not accepts<Word> (" \t\nx", Word::ConsumeRemainingLine | Word::PrecedingLinebreakNotAllowed));
  QVERIFY (    accepts<Word> (" \t\nx", Word::ConsumeRemainingLine |  Word::PrecedingLinebreakRequired));
  
  
  
  
  QVERIFY (    accepts<Word> (" \tx"));
  QVERIFY (    accepts<Word> (" \tx", Word::PrecedingLinebreakNotAllowed));
  QVERIFY (not accepts<Word> (" \tx", Word::PrecedingLinebreakRequired));
  
  
  QVERIFY (    accepts<Word> ("\tx \t", Word::ConsumeRemainingLine));
  QVERIFY (    accepts<Word> ("\tx \t", Word::ConsumeRemainingLine | Word::PrecedingLinebreakNotAllowed));
  QVERIFY (not accepts<Word> ("\tx \t", Word::ConsumeRemainingLine | Word::PrecedingLinebreakRequired));
  
  QVERIFY (not accepts<Word> ("\tx \t"));
  QVERIFY (not accepts<Word> ("\tx \t", Word::PrecedingLinebreakNotAllowed));
  QVERIFY (not accepts<Word> ("\tx \t", Word::PrecedingLinebreakRequired));
  
  QVERIFY (reproduces<Word> (" \t\nx"));
  QVERIFY (reproduces<Word> ("\tx \t", Word::ConsumeRemainingLine));
}



void
Obj8::Test::Parser::endOfLine ()
{
  QVERIFY (    accepts<Word> ("\nFoobar", Obj8::Parser::LexerContext::IBM));
  QVERIFY (    accepts<Word> ("\r\nFoobar", Obj8::Parser::LexerContext::IBM));
  QVERIFY (    accepts<Word> ("\rFoobar", Obj8::Parser::LexerContext::IBM));
  QVERIFY (not accepts<Word> ("Foobar\n", Obj8::Parser::LexerContext::IBM));
  QVERIFY (not accepts<Word> ("Foobar\r\n", Obj8::Parser::LexerContext::IBM));
  QVERIFY (    accepts<Word> ("Foobar\r", Obj8::Parser::LexerContext::IBM));
  
  QVERIFY (    accepts<Word> ("\nFoobar", Obj8::Parser::LexerContext::Apple));
  QVERIFY (    accepts<Word> ("\r\nFoobar", Obj8::Parser::LexerContext::Apple));
  QVERIFY (    accepts<Word> ("\rFoobar", Obj8::Parser::LexerContext::Apple));
  QVERIFY (    accepts<Word> ("Foobar\n", Obj8::Parser::LexerContext::Apple));
  QVERIFY (not accepts<Word> ("Foobar\r\n", Obj8::Parser::LexerContext::Apple));
  QVERIFY (not accepts<Word> ("Foobar\r", Obj8::Parser::LexerContext::Apple));
}



void
Obj8::Test::Parser::lexerContext ()
{
  Obj8::Parser::LexerContext ctx;
  Obj8::String text = "The quick brown fox jumps";
  Obj8::Parser::Word (text, &ctx);
  QVERIFY ((ctx.line() == 1) and (ctx.column() == 4));
  Obj8::Parser::Word (text, &ctx);
  QVERIFY ((ctx.line() == 1) and (ctx.column() == 10));
  Obj8::Parser::Word (text, &ctx);
  QVERIFY ((ctx.line() == 1) and (ctx.column() == 16));
  Obj8::Parser::Word (text, &ctx);
  QVERIFY ((ctx.line() == 1) and (ctx.column() == 20));
  Obj8::Parser::Word (text, &ctx);
  QVERIFY ((ctx.line() == 1) and (ctx.column() == 26));
}



void
Obj8::Test::Parser::addsub (QString a, QString b, QString result, bool add)
{
  Number x(a);
  Number y(b);
  Number z;
  
  if (add) {
    z = x+y;
  } else {
    z = x-y;
  }
  
  QCOMPARE (z.toString(), result);
}



void
Obj8::Test::Parser::math ()
{
  Number n, m, o;
  
  /*
   * sign change and parsing
   */
  n = Number ("0.341");
  m = Number ("-.342");
  o = n + m;
  
  QVERIFY (n.numerator() == 341);
  QVERIFY (n.denominator() == 1000);
  QVERIFY (qFuzzyCompare (n.toDouble(), 0.341));
  QVERIFY (n.toInt() == 0);
  
  QVERIFY (m.numerator() == -342);
  QVERIFY (m.denominator() == 1000);
  QVERIFY (qFuzzyCompare (m.toDouble(), -0.342));
  QVERIFY (n.toInt() == 0);
  
  QVERIFY (o.numerator() == -1);
  QVERIFY (o.denominator() == 1000);
  QVERIFY (qFuzzyCompare (o.toDouble(), -0.001));
  QVERIFY (n.toInt() == 0);
  
  /*
   * negate
   */
  o = -m;
  QVERIFY (o.numerator() == 342);
  QVERIFY (o.denominator() == 1000);
  
  /*
   * different number of digits
   */
  addsub ("0.341", "10", "10.341", true);
  addsub ("10", "0.341", "10.341", true);
  
  /*
   * overflow
   */
  addsub ("5.0",  "6.0",  "11.0", true);
  addsub ("5.5", "94.5", "100.0", true);
  addsub ( "-5",   "-6", "-11", true);
  
  
  /*
   * substraction
   */
  addsub ( "0.1",  "9.9",  "-9.8", false);
  addsub ( "9.9",  "1.9",   "8.0", false);
  addsub ("-0.1",  "9.9", "-10.0", false);
  addsub ( "0.1", "-9.9",  "10.0", false);
  addsub ("-0.1", "-9.9",   "9.8", false);
  
  /*
   * keep formating
   */
  addsub ("  0.1", "-9.9", "  10.0", false);
  addsub ("000.1", "-9.9",  "010.0", false);
}



void
Obj8::Test::Parser::generic ()
{
  Number n;
  Obj8::String str = n.toString ();
  QVERIFY (str == "0");
  
  
  QChar br(Obj8::Parser::LexerContext::ShortEndOfLine);
  Word a, b, c;
  
  a = Word ("  \nHello");
  QVERIFY (a.indentation() == QString("  %1").arg(br));
  QVERIFY (a.value() == "Hello");
  b = a;
  QVERIFY (b.indentation() == QString("  %1").arg(br));
  QVERIFY (b.value() == "Hello");
  c = Word ("\t\nWorld!");
  QVERIFY (c.indentation() == QString("\t%1").arg(br));
  QVERIFY (c.value() == "World!");
  
  a.append (c);
  QVERIFY (a.indentation() == QString("  %1").arg(br));
  QVERIFY (a.value() == QString("Hello\t%1World!").arg(br));
  QVERIFY (b.indentation() == QString("  %1").arg(br));
  QVERIFY (b.value() == QString("Hello"));
}

QTEST_MAIN (Obj8::Test::Parser);
