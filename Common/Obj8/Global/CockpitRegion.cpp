#include "CockpitRegion.hpp"




const unsigned int m[] = {
  0x55555555,
  0x33333333,
  0x0f0f0f0f,
  0x00ff00ff,
  0x0000ffff
};



static unsigned int hamming_weight (unsigned int i)
{
  for (size_t n=0; n!=nitems(m); ++n) {
    i = (i & m[n]) + ((i >> (1 << n)) & m[n]);
  }
  return (i);
}



Obj8::Global::CockpitRegion::CockpitRegion ()
  : Record ()
  , m_left ()
  , m_bottom ()
  , m_right ()
  , m_top ()
{
}




Obj8::Global::CockpitRegion::CockpitRegion (StringRef str, Parser::LexerContext *ctx)
  : Record (str, ctx)
  , m_left (str, ctx)
  , m_bottom (str, ctx)
  , m_right (str, ctx)
  , m_top (str, ctx)
{
  int left, right, bottom, top;
  unsigned int wbits, hbits;
  
  left   = m_left.toInt ();
  right  = m_right.toInt ();
  
  bottom = m_bottom.toInt ();
  top    = m_top.toInt ();
  
  /*
   * integer overflow
   */
  if (right < left) {
    m_right.reject ();
  }
  
  if (top < bottom) {
    m_bottom.reject ();
  }
  
  /*
   * power of two dimension. 0 is ok?
   */
  
  wbits = hamming_weight (right - left);
  hbits = hamming_weight (top - bottom);
  
  if (wbits > 1) {
    m_right.reject ();
  }
  if (hbits > 1) {
    m_top.reject ();
  }
}



Obj8::Global::CockpitRegion::~CockpitRegion ()
{
}



void
Obj8::Global::CockpitRegion::accept (AbstractVisitor *, bool)
{
}



Obj8::RecordPointer
Obj8::Global::CockpitRegion::instantiate (StringRef str, Parser::LexerContext *ctx) const
{
  return (RecordPointer (new CockpitRegion (str, ctx)));
}



Obj8::String
Obj8::Global::CockpitRegion::name () const
{
  return ("COCKPIT_REGION");
}



Obj8::String
Obj8::Global::CockpitRegion::toString () const
{
  String retval;
  
  retval  = Record::toString ();
  retval += m_left.toString ();
  retval += m_bottom.toString ();
  retval += m_right.toString ();
  retval += m_top.toString ();
  
  return (retval);
}
