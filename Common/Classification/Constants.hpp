#pragma once

#include <stddef.h>
#include <iso646.h>

#include <QtCore/QMetaEnum>
#include <QtCore/QString>
#include <QtCore/QStringList>

namespace Classification
{
  template<typename Enumeration>
  Enumeration
  stringToEnum (const QString &string, const Enumeration &default_value =Enumeration())
  {
    Enumeration retval;
    bool ok;
    
    QMetaEnum me = QMetaEnum::fromType<Enumeration>();
    retval = (Enumeration)me.keysToValue(qUtf8Printable(string), &ok);
    
    if (not ok) {
      retval = default_value;
    }
    
    return (retval);
  }
  
  template<typename Enumeration>
  Enumeration
  stringToEnum (const QVariant &v, const Enumeration &default_value =Enumeration())
  {
    return (stringToEnum(v.toString(), default_value));
  }
  
  
  template<typename Enumeration>
  QString
  enumToString (const int &value)
  {
    QByteArray string;
    
    QMetaEnum me = QMetaEnum::fromType<Enumeration>();
    string = me.valueToKeys (value);
    
    Q_ASSERT (not string.isEmpty());
    
    return (string);
  }
  
  
  struct Constants
  {
    typedef int Length;
    typedef int Weight;
    
    enum Size {
      SizeA, SizeB, SizeC, SizeD, SizeE, SizeF
    };
    
    enum Engine {
      Jet, Turbofan, Turboprop, Piston, Helicopter
    };
    
    enum Category {
      Airliner, Cargo, GeneralAviation, Military, MultiPurpose
    };
    
    static Length g_lenght_of_meter;
    static Weight g_weight_of_ton;
    
    static Size    size (Length);
    //static QString xplaneName (Category, Engine, Size);
    
    static QString engine (Engine);
    static Engine  engine (QString);
    static size_t  numberOfEngineTypes ();
  };
}
