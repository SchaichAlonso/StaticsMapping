#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QString>

#include "Classification.hpp"

namespace Classification
{
  struct Record : QObject
  {
    typedef QString PrimaryKey;
    
    Record ();
    Record (const Record &);
    Record (Definitions *, const QJsonObject &);
    virtual ~Record ();
    
    virtual QJsonObject toJson () const = 0;
   
    virtual const char *primaryKeyProperty () const = 0;
    virtual bool verifyPrimaryKey (PrimaryKey) const = 0;
    PrimaryKey primaryKey () const;
    
    void setDefinitions (Definitions *);
    QList<const char *> properties () const;
    
  protected:
    virtual void fixReferences ();
    
    Definitions *m_q;
  };
}

/*
 * forwarder to the QPropertySystem
 */
#define PROPERTY(X,Y) \
  X::propertyByName(X::Y##Property)
