#include "Aircraft.hpp"
#include "Airline.hpp"
#include "Definitions.hpp"
#include "ObjectModel.hpp"
#include "Object.hpp"

#include <QtCore/QMetaProperty>

Classification::ObjectModel::ObjectModel (Definitions *definitions)
  : Model (definitions, Object())
{
}



Classification::ObjectModel::~ObjectModel ()
{
}



int
Classification::ObjectModel::column (int name) const
{
  Object::PropertyName pn = (Object::PropertyName)name;
  const char *string = Object::propertyByName (pn);
  return (Model::column (string));
}



Classification::QObjectPointer
Classification::ObjectModel::record (int row) const
{
  return (m_definitions->object (row));
}



int
Classification::ObjectModel::rowCount (const QModelIndex &) const
{
  int retval;
  
  if (m_definitions) {
    retval = m_definitions->numberOfObjects ();
  } else {
    retval = 0;
  }
  
  return (retval);
}



QVariant
Classification::ObjectModel::data (QObjectPointer ptr, const char *property) const
{
  if (0 == qstrcmp (property, PROPERTY(Object,Purpose))) {
    return (enumToString<Object::Purpose>(ptr->property(property).toInt()));
  }
  
  if (0 == qstrcmp (property, PROPERTY(Object,Rotate))) {
    return (enumToString<Object::Rotate>(ptr->property(property).toInt()));
  }
  
  return (Model::data (ptr, property));
}



bool
Classification::ObjectModel::setData (QObjectPointer ptr, const char *property, const QVariant &value)
{
  ObjectPointer obj = qSharedPointerDynamicCast<Object>(ptr);
  
  Q_ASSERT (obj);
  
  bool ok = obj->setProperty (property, value);
  
  if (ok) {
    m_definitions->upsert (obj);
  }
  
  return (ok);
}
