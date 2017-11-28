#pragma once

#include <QJsonObject>
#include <QString>

#include "Record.hpp"

namespace Classification
{
  struct IcaoRecord : Record
  {
    Q_OBJECT
    
    Q_PROPERTY (QString icao READ icao)
    Q_PROPERTY (QString name MEMBER m_name)
    
  public:
    IcaoRecord (const IcaoRecord &);
    IcaoRecord (QString, QString);
    IcaoRecord (Definitions *, const QJsonObject &);
    virtual ~IcaoRecord ();
    
    virtual const char *primaryKeyProperty () const Q_DECL_OVERRIDE;
    
    bool verifyIcaoCode (QString) const;
    bool hasFictiveIcaoCode() const;
    
    QString icao () const;
    void setIcao (QString);
    
    QString name () const;
    void setName (QString);
    
    virtual QJsonObject toJson () const Q_DECL_OVERRIDE;
    
  protected:
    QString m_icao, m_name;
  };
}
