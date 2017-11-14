#include "Constants.hpp"




template<typename A, typename B>
struct MappingElement
{
  A name;
  B string;
};

#define MAPPING_ELEMENT(X, x) { X::x, #x }
#ifndef N_ELEMS /* from sys/types.h */
#  define N_ELEMS(x) (sizeof(x)/sizeof(*x))
#endif

static MappingElement<Classification::Constants::Engine, QString> __engines[] = {
  MAPPING_ELEMENT(Classification::Constants, Jet),
  MAPPING_ELEMENT(Classification::Constants, Turbofan),
  MAPPING_ELEMENT(Classification::Constants, Turboprop),
  MAPPING_ELEMENT(Classification::Constants, Piston),
  MAPPING_ELEMENT(Classification::Constants, Helicopter)
};



static MappingElement<Classification::Constants::Category, QString> __xp_category_vpath[] =
{
  { Classification::Constants::Airliner, "airliners" },
  { Classification::Constants::Cargo, "cargo" },
  { Classification::Constants::GeneralAviation, "general_aviation" },
  { Classification::Constants::Military, "military" }
};


Classification::Constants::Length
Classification::Constants::g_lenght_of_meter = 1000;



Classification::Constants::Weight
Classification::Constants::g_weight_of_ton = 1000;





#if 0
QString
Classification::Constants::xplaneName (Object::Usage c, Engine e, Size s)
{
  QString xpcat, xptype;
  QChar size;
  
  Q_ASSERT ((size_t)c < N_ELEMS(__xp_category_vpath));
  Q_ASSERT (__xp_category_vpath[c].name == c);
  xpcat = __xp_category_vpath[c].string;
  
  switch (e) {
    case Jet:
    case Turbofan:
      xptype = (s > SizeC)? "heavy_" : "jet_";
      break;
    case Turboprop:
      xptype = "turboprop_";
      break;
    case Piston:
      xptype = "prop_";
      break;
    case Helicopter:
      xptype = "helicopter_";
      break;
  }
  
  size = QChar('a' + (s - SizeA));
  xptype.append(size);
  
  return (QString("lib/airport/aircraft/%1/%2").arg(xpcat).arg(xptype));
}
#endif


QString
Classification::Constants::engine (Engine e) {
  Q_ASSERT ((size_t)e < N_ELEMS(__engines));
  Q_ASSERT (__engines[e].name == e);
  return (__engines[e].string);
}


Classification::Constants::Engine
Classification::Constants::engine (QString s) {

  const MappingElement<Engine,QString> *i, *e;

  for (i=__engines, e=i+N_ELEMS(__engines); i!=e; ++i) {
    if (i->string.compare(s, Qt::CaseInsensitive) == 0)
      return (i->name);
  }

  throw "no such value";
}



size_t
Classification::Constants::numberOfEngineTypes () {
  return (N_ELEMS(__engines));
}

