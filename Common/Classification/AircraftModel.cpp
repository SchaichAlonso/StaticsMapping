#include <Obj8/Parser/Number.hpp>

#include "Aircraft.hpp"
#include "AircraftModel.hpp"
#include "Definitions.hpp"



Classification::AircraftModel::AircraftModel (Definitions *definitions)
  : Model (definitions, Aircraft())
{
}



Classification::AircraftModel::~AircraftModel ()
{
}


int
Classification::AircraftModel::column (int name) const
{
  Aircraft::PropertyName pn = (Aircraft::PropertyName)name;
  const char *string = Aircraft::propertyByName (pn);
  return (Model::column (string));
}



Classification::RecordPointer
Classification::AircraftModel::record (int row) const
{
  return (m_definitions->aircraft (row));
}



int
Classification::AircraftModel::rowCount (const QModelIndex &) const
{
  if (m_definitions) {
    return (m_definitions->numberOfAircrafts());
  }
  
  return (0);
}


QVariant
Classification::AircraftModel::data (RecordPointer ptr, const char *property) const
{
  if (0 == qstrcmp (property, PROPERTY(Aircraft,Engine))) {
    return (enumToString<Aircraft::Engine>(ptr->property(property).toInt()));
  }
  
  bool mtow = (0 == qstrcmp (property, PROPERTY(Aircraft,MTOW)));
  
  if (mtow
      or (0 == qstrcmp (property, PROPERTY(Aircraft,Length)))
      or (0 == qstrcmp (property, PROPERTY(Aircraft,OMGWS)))
      or (0 == qstrcmp (property, PROPERTY(Aircraft,Wingspan)))) {
    Obj8::Parser::Number n;
    Obj8::Parser::Number::Denominator d;
   
    d = mtow? Aircraft::g_weight_of_ton : Aircraft::g_length_of_meter;
  
    n.setNumerator (ptr->property(property).toInt());
    n.setDenominator (d);
    return (n.toString ());
  }
  
  return (Model::data (ptr, property));
}



bool
Classification::AircraftModel::setData (RecordPointer p, const char *property, const QVariant &value)
{
  AircraftPointer ptr = qSharedPointerDynamicCast<Aircraft>(p);
  bool mtow, ok;
  
  Q_ASSERT (ptr);
  
  mtow = (0 == qstrcmp (property, PROPERTY(Aircraft,MTOW)));
  
  if (mtow
      or (0 == qstrcmp (property, PROPERTY(Aircraft,Length)))
      or (0 == qstrcmp (property, PROPERTY(Aircraft,OMGWS)))
      or (0 == qstrcmp (property, PROPERTY(Aircraft,Wingspan)))) {
    
    Obj8::Parser::Number n (value.toString());
    Obj8::Parser::Number::Denominator d;
    
    ok = n.isValid ();
    if (ok) {
      ok = (n.numerator() >= 0);
      if (ok) {
        d = mtow? Aircraft::g_weight_of_ton : Aircraft::g_length_of_meter;
        n.toDenominator (d);
        ok = ptr->setProperty (property, n.numerator());
      }
    }
  } else {
    ok = ptr->setProperty (property, value);
  }
  
  if (ok) {
    m_definitions->upsert (ptr);
  }
  
  return (ok);
}
