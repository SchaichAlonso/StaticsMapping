#pragma once

#include <QJsonObject>
#include <QString>

#include "Constants.hpp"
#include "IcaoRecord.hpp"

namespace Classification
{
  struct Aircraft : IcaoRecord
  {
    Q_OBJECT
    
    Q_PROPERTY (Engine engine       MEMBER m_engine);
    Q_PROPERTY (int    mtow         MEMBER m_mtow);
    Q_PROPERTY (int    length       MEMBER m_length);
    Q_PROPERTY (int    wingspan     MEMBER m_wingspan);
    Q_PROPERTY (int    omgws        MEMBER m_wheelspan);
    Q_PROPERTY (int    introduction MEMBER m_introduced);
  
  public:
    enum PropertyName {
      IcaoProperty,
      NameProperty,
      
      EngineProperty,
      MTOWProperty,
      LengthProperty,
      WingspanProperty,
      OMGWSProperty,
      FirstFlightProperty
    };
    static const char *propertyByName (PropertyName);
    
  public:
    
    typedef int Length;
    typedef int Weight;
    
    static Length g_length_of_meter;
    static Weight g_weight_of_ton;
    
    enum Size {
      SizeA, SizeB, SizeC, SizeD, SizeE, SizeF
    };
    enum Engine {
      Jet, Turbofan, Turboprop, Piston, Helicopter
    };
    Q_ENUM (Size);
    Q_ENUM (Engine);
    
    Aircraft (const Aircraft &);
    Aircraft (QString =QString(), QString =QString());
    Aircraft (Definitions *, QJsonObject);
    virtual ~Aircraft ();
    
    virtual QJsonObject toJson() const Q_DECL_OVERRIDE;
    
    virtual bool verifyPrimaryKey (PrimaryKey) const Q_DECL_OVERRIDE;
    
    Engine   engine () const;
    void   setEngine (Engine);
    
    Weight mtow () const;
    void   setMtow (Weight);
    
    Length length () const;
    void   setLength (Length);
    
    Length wheelspan () const;
    void   setWheelspan (Length);
    
    Length wingspan () const;
    void   setWingspan (Length);
    
    int    introduced () const;
    void   setIntroduced (int);
    
    Size   size() const;
    
  protected:
    Length  m_length, m_wheelspan, m_wingspan;
    Weight  m_mtow;
    int     m_introduced;
    Engine  m_engine;
  };
}
