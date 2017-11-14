#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>

#include "Record.hpp"



Classification::Record::Record ()
: QObject ()
, m_q (Q_NULLPTR)
{
}



Classification::Record::Record (const Record &o)
: QObject ()
, m_q (o.m_q)
{
}



Classification::Record::Record (Definitions *q, const QJsonObject &)
: QObject ()
, m_q (q)
{
  Q_CHECK_PTR (m_q);
}



Classification::Record::~Record ()
{
}


QJsonObject
Classification::Record::toJson () const
{
  QJsonObject o;
  
  return (o);
}


void
Classification::Record::setDefinitions (Definitions *definitions)
{
  m_q = definitions;
  fixReferences ();
}



void
Classification::Record::fixReferences ()
{
}


Classification::Record::PrimaryKey
Classification::Record::primaryKey () const
{
  const char *p = primaryKeyProperty ();
  return (property(p).toString());
}



QList<const char *>
Classification::Record::properties () const
{
  QList<const char *> retval;
  const QMetaObject *obj = metaObject ();
  
  for (int i=0, k=obj->propertyCount(); i!=k; ++i) {
    QMetaProperty mp = obj->property(i);
    const char *name = mp.name();
    if (qstrcmp("objectName", name)) {
      retval.append (name);
    }
  }
  
  return (retval);
}
