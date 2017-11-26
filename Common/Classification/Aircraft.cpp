#include <QtCore/QRegExp>
#include "Aircraft.hpp"


Classification::Aircraft::Length
Classification::Aircraft::g_length_of_meter = 1000;



Classification::Aircraft::Weight
Classification::Aircraft::g_weight_of_ton = 1000;


const char *
Classification::Aircraft::propertyByName (PropertyName p)
{
  switch (p) {
    case IcaoProperty: return ("icao");
    case NameProperty: return ("name");
    
    case EngineProperty: return ("engine");
    case MTOWProperty: return ("mtow");
    case LengthProperty: return ("length");
    case WingspanProperty: return ("wingspan");
    case OMGWSProperty: return ("omgws");
    case FirstFlightProperty: return ("introduction");
  }
  
  abort ();
}


Classification::Aircraft::Aircraft (const Aircraft &o)
  : IcaoRecord (o)
  , m_length (o.m_length)
  , m_wheelspan (o.m_wheelspan)
  , m_wingspan (o.m_wingspan)
  , m_mtow (o.m_mtow)
  , m_introduced (o.m_introduced)
  , m_engine (o.m_engine)
{
}



Classification::Aircraft::Aircraft (QString icao, QString name)
  : IcaoRecord (icao, name)
  , m_length (0)
  , m_wheelspan (0)
  , m_wingspan (0)
  , m_mtow (0)
  , m_introduced (0)
  , m_engine (Piston)
{
}



Classification::Aircraft::Aircraft (Definitions *q, QJsonObject o)
  : IcaoRecord (q, o)
  , m_length (o.value("length").toInt())
  , m_wheelspan (o.value("wheelspan").toInt())
  , m_wingspan (o.value("wingspan").toInt())
  , m_mtow (o.value("mtow").toInt())
  , m_introduced (o.value("introduced").toInt())
  , m_engine (stringToEnum<Engine>(o.value("engine"), Piston))
{
}



Classification::Aircraft::~Aircraft()
{
}



QJsonObject
Classification::Aircraft::toJson () const
{
  QJsonObject obj = IcaoRecord::toJson ();

  obj.insert ("wheelspan", m_wheelspan);
  obj.insert ("wingspan", m_wingspan);
  obj.insert ("mtow", m_mtow);
  obj.insert ("length", m_length);
  obj.insert ("introduced", m_introduced);
  obj.insert ("engine", enumToString<Engine>(m_engine));

  return (obj);
}



bool
Classification::Aircraft::verifyPrimaryKey (PrimaryKey icao) const
{
  QRegExp expr ("[A-Z][A-Z0-9]{1,3}");
  bool retval;
  
  retval = expr.exactMatch (icao);
  
  return (retval);
}



Classification::Aircraft::Engine
Classification::Aircraft::engine () const
{
  return (m_engine);
}



void
Classification::Aircraft::setEngine (Engine e)
{
  m_engine = e;
}



Classification::Aircraft::Weight
Classification::Aircraft::mtow () const
{
  return (m_mtow);
}



void
Classification::Aircraft::setMtow (Weight mtow)
{
  m_mtow = mtow;
}



Classification::Aircraft::Length
Classification::Aircraft::length () const
{
  return (m_length);
}



void
Classification::Aircraft::setLength (Length x)
{
  m_length = x;
}



Classification::Aircraft::Length
Classification::Aircraft::wheelspan () const
{
  return (m_wheelspan);
}



void
Classification::Aircraft::setWheelspan (Length s)
{
  m_wheelspan = s;
}



Classification::Aircraft::Length
Classification::Aircraft::wingspan () const
{
  return (m_wingspan);
}



void
Classification::Aircraft::setWingspan (Length s)
{
  m_wingspan = s;
}



int
Classification::Aircraft::introduced () const
{
  return (m_introduced);
}


void
Classification::Aircraft::setIntroduced (int year)
{
  m_introduced = year;
}


Classification::Aircraft::Size
Classification::Aircraft::size () const
{
  /*
   * http://aai.aero/aai_employees/chapter_2.pdf
   */
#define mm(x) \
  (x * g_length_of_meter / 1000)
  if ((m_wingspan < mm(15000)) and (m_wheelspan <  mm(4500)))
    return (SizeA);
  if ((m_wingspan < mm(24000)) and (m_wheelspan <  mm(6000)))
    return (SizeB);
  if ((m_wingspan < mm(36000)) and (m_wheelspan <  mm(9000)))
    return (SizeC);
  if ((m_wingspan < mm(52000)) and (m_wheelspan < mm(14000)))
    return (SizeD);
  if ((m_wingspan < mm(65000)) and (m_wheelspan < mm(14000)))
    return (SizeE);
  if ((m_wingspan < mm(80000)) and (m_wheelspan < mm(16000)))
    return (SizeF);
  
  /*
   * exception?
   */
  qWarning ("Size of Superplane %s truncated", qUtf8Printable(primaryKey()));
  return (SizeF);
}
