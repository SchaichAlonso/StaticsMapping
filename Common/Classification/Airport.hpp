#pragma once

#include <QJsonObject>
#include <QString>

#include "IcaoRecord.hpp"

namespace Classification
{
  struct Airport : IcaoRecord
  {
    typedef double Degrees;
    typedef double Radians;
    typedef double DistanceInRadians;
    typedef int    DistanceInKM;
    
    Airport (QString icao=QString(), QString name=QString(), Degrees lat=0, Degrees lon=0);
    Airport (Definitions *, const QJsonObject &);
    virtual ~Airport ();
    
    virtual bool verifyPrimaryKey (PrimaryKey) const Q_DECL_OVERRIDE;
    
    DistanceInRadians distance (const Airport &) const;
    DistanceInKM      distanceInKM (const Airport &) const;
    
    static DistanceInKM earthRadiusInKM ();
    static DistanceInKM largestPossibleDistanceInKM ();
    
    void setLatitude (Degrees lat);
    Degrees latitude () const;
    
    void setLongitude (Degrees lon);
    Degrees longitude () const;
    
    virtual QJsonObject toJson () const Q_DECL_OVERRIDE;
    
  protected:
    Degrees  m_lat, m_lon;
  };
}
