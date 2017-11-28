#include <QtCore/QMetaProperty>

#include "Airline.hpp"
#include "AirlineModel.hpp"
#include "Airport.hpp"
#include "Definitions.hpp"



Classification::AirlineModel::AirlineModel (Definitions *definitions)
  : Model (definitions, Airline())
{
  /*
   * The model is reset only if new data was added or present one is removed,
   * both are structural changes.
   */
  connect (
      this, &AirlineModel::modelReset,
      this, &AirlineModel::structuralChange
  );
}



Classification::AirlineModel::~AirlineModel ()
{
}



int
Classification::AirlineModel::column (int name) const
{
  Airline::PropertyName pn = (Airline::PropertyName)name;
  const char *string = Airline::propertyByName (pn);
  return (Model::column (string));
}



Classification::QObjectPointer
Classification::AirlineModel::record (int row) const
{
  return (m_definitions->airline (row));
}



int
Classification::AirlineModel::rowCount (const QModelIndex &) const
{
  if (m_definitions) {
    return (m_definitions->numberOfAirlines());
  }
  
  return (0);
}



bool
Classification::AirlineModel::setData (QObjectPointer p, const char *property, const QVariant &value)
{
  AirlinePointer ptr = qSharedPointerDynamicCast<Airline>(p);
  bool commited, structural_change = false;
  
  Q_ASSERT (ptr);
  
  if (0 == qstrcmp (property, PROPERTY(Airline,Parent))) {
    QString old_val, new_val;
    
    old_val = ptr->property (property).toString ();
    new_val = value.toString ();
    if (old_val != new_val) {
      commited = ptr->setProperty (property, value);
      structural_change = commited;
    } else {
      commited = false;
    }
  } else {
    commited = Model::setData (p, property, value);
  }
  
  if (commited) {
    m_definitions->upsert (ptr);
  }
  
  if (structural_change) {
    emit structuralChange ();
  }
  
  return (commited);
}
