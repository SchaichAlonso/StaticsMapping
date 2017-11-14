#pragma once

#include <QtCore/QMultiMap>
#include <QtCore/QList>
#include <QtCore/QString>

#include "Aircraft.hpp"
#include "Classification.hpp"
#include "Definitions.hpp"
#include "Object.hpp"

namespace Classification
{
  struct XPClass
  {
    enum Purpose {
      GeneralAviation,
      Airliner,
      Cargo,
      Military
    };
    enum Engine {
      Piston,
      Turbofan,
      Turboprop,
      Helicopter,
      Fighter
    };
    typedef Aircraft::Size Size;
    
    static QList<XPClass> create (Object::Purposes, Aircraft::Engine, Aircraft::Size);
    static QList<XPClass> create (AircraftPointer, ObjectPointer);
    
    XPClass (Object::Purposes, Aircraft::Engine, Aircraft::Size);
    XPClass ();
    
    bool couldBeLarger () const;
    XPClass nextLarger () const;
    
    QString libraryVPath (QString =QString()) const;
    QString name () const;
    
    bool operator< (const XPClass &) const;
    bool operator== (const XPClass &) const;
    bool operator!= (const XPClass &) const;
    
#if 0
    friend uint qHash (const XPData &d) {
      return (::qHash (QByteArray::fromRawData((const char *)(&d), sizeof(d))));
    }
#endif
    
  protected:
    Purpose m_purpose;
    Engine  m_engine;
    Size    m_size;
    
    static Engine  engine (Object::Purposes, Aircraft::Engine, Aircraft::Size);
    static Purpose purpose (Object::Purposes, Aircraft::Engine, Aircraft::Size);
    QString engineString () const;
    QString purposeString () const;
    QString sizeString () const;
  };
  
  typedef QMultiMap<XPClass, ObjectPointer> ObjectsByXPClass;
}
