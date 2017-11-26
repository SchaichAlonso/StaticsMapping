#include "Object.hpp"
#include "XPClass.hpp"


QList<Classification::XPClass>
Classification::XPClass::create (Object::Purposes p, Aircraft::Engine t, Aircraft::Size s)
{
  QMap<Classification::XPClass, int> map;
  
  for (int i=0; i!=(sizeof(p)*CHAR_BIT); ++i) {
    if (p & (1 << i)) {
      try {
        map.insert (XPClass(Object::Purposes(1<<i), t, s), 0);
      } catch (...) {
      }
    }
  }
  
  return (map.keys());
}



QList<Classification::XPClass>
Classification::XPClass::create (AircraftPointer a, ObjectPointer o)
{
  QList<Classification::XPClass> retval;
  
  Q_ASSERT (a->primaryKey() == o->aircraft());
  
  if (a) {
    retval = create (o->purpose(), a->engine(), a->size());
  }
  
  return (retval);
}



Classification::XPClass::XPClass ()
  : m_purpose ()
  , m_engine ()
  , m_size ()
{
}



Classification::XPClass::XPClass (Object::Purposes p, Aircraft::Engine t, Aircraft::Size s)
  : m_purpose (purpose(p, t, s))
  , m_engine (engine(p,t,s))
  , m_size (s)
{
}



bool
Classification::XPClass::couldBeLarger () const
{
  return (m_size != Aircraft::SizeF);
}



Classification::XPClass
Classification::XPClass::nextLarger () const
{
  XPClass retval (*this);
  
  if (m_size != Aircraft::SizeF) {
    retval.m_size = Size(m_size + 1);
  }
  
  return (retval);
}



QString
Classification::XPClass::libraryVPath (QString airline) const
{
  QString retval;
  
  retval = name ();
  if (not airline.isNull()) {
    retval.append ("_");
    retval.append (airline.toLower());
  }
  
  retval.append (".obj");
  
  return (retval);
}



QString
Classification::XPClass::name () const
{
  QString retval;
  
  retval = QString ("lib/airport/aircraft/%1/%2_%3")
      .arg (purposeString ())
      .arg (engineString ())
      .arg (sizeString ());
  
  return (retval);
}



Classification::XPClass::Engine
Classification::XPClass::engine (
    Object::Purposes purpose,
    Aircraft::Engine engine,
    Aircraft::Size size
)
{
  Engine retval;
  
  switch (engine) {
    case Aircraft::Jet:
    case Aircraft::Turbofan:
      retval = Turbofan;
      break;
    case Aircraft::Turboprop:
      retval = Turboprop;
      break;
    case Aircraft::Piston:
      retval = Piston;
      break;
    case Aircraft::Helicopter:
      retval = Helicopter;
      break;
  }
  
  /*
   * rewrite 'fighter'
   */
  switch (purpose) {
    case Object::GeneralAviation:
    case Object::Airliner:
    case Object::Freighter:
      break;
    
    case Object::Bomber:
    case Object::ElectronicWarfare:
    case Object::MaritimePatrol:
    case Object::AEW:
    case Object::Tanker:
    case Object::MilitaryTransport:
    case Object::Trainer:
      break;
    
    case Object::Fighter:
      retval = Fighter;
      break;
    
    case Object::Attack:
    case Object::Reconnaisance:
    case Object::Surveillance:
      retval = (size <= Aircraft::SizeB)? Fighter : retval;
      break;
  }
  
  return (retval);
}



Classification::XPClass::Purpose
Classification::XPClass::purpose (Object::Purposes p, Aircraft::Engine, Aircraft::Size)
{
  switch (p) {
    case Object::GeneralAviation:   return (GeneralAviation);
    case Object::Airliner:          return (Airliner);
    case Object::Freighter:         return (Cargo);
    
    case Object::Attack:            return (Military);
    case Object::Bomber:            return (Military);
    case Object::ElectronicWarfare: return (Military);
    case Object::Fighter:           return (Military);
    case Object::MaritimePatrol:    return (Military);
    case Object::AEW:               return (Military);
    case Object::Reconnaisance:     return (Military);
    case Object::Surveillance:      return (Military);
    case Object::Tanker:            return (Military);
    case Object::Trainer:           return (Military);
    case Object::MilitaryTransport: return (Military);
  }
  abort ();
}



QString
Classification::XPClass::engineString () const
{
  switch (m_engine) {
    case Piston:     return ("prop");
    case Turboprop:  return ("turboprop");
    case Turbofan:   return ((m_size < Aircraft::SizeD)? "jet" : "heavy");
    case Helicopter: return ("helo");
    case Fighter:    return ("fighter");
  }
}


QString
Classification::XPClass::purposeString () const
{
  switch (m_purpose) {
    case GeneralAviation: return ("general_aviation");
    case Airliner:        return ("airliners");
    case Cargo:           return ("cargo");
    case Military:        return ("military");
  }
}



QString
Classification::XPClass::sizeString () const
{
  char str[2];
  
  str[0] = 'a' + (m_size - Aircraft::SizeA);
  str[1] = 0;
  
  return (str);
}



bool
Classification::XPClass::operator< (const XPClass &o) const
{
  if (m_purpose != o.m_purpose) {
    return (m_purpose < o.m_purpose);
  }
  
  if (m_engine != o.m_engine) {
    return (m_engine < o.m_engine);
  }
  
  return (m_size < o.m_size);
}



bool
Classification::XPClass::operator== (const XPClass &o) const
{
  return (not (*this != o));
}



bool
Classification::XPClass::operator!= (const XPClass &o) const
{
  return ((*this < o) or (o < *this));
}


