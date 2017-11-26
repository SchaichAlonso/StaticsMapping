#include <QtCore/QtMath>

#include "Airport.hpp"

Classification::Airport::Airport (QString icao, QString name, Degrees lat, Degrees lon)
  : IcaoRecord (icao, name)
  , m_lat (lat)
  , m_lon (lon)
{
}



Classification::Airport::Airport (Definitions *q, const QJsonObject &o)
  : IcaoRecord (q, o)
  , m_lat (o.value("lat").toDouble())
  , m_lon (o.value("lon").toDouble())
{
}



Classification::Airport::~Airport ()
{
}



bool
Classification::Airport::verifyPrimaryKey (PrimaryKey) const
{
  return (true);
}



Classification::Airport::DistanceInRadians
Classification::Airport::distance (const Airport &other) const
{
  Radians tlat = qDegreesToRadians (m_lat);
  Radians tlon = qDegreesToRadians (m_lon);
  Radians olat = qDegreesToRadians (other.m_lat);
  Radians olon = qDegreesToRadians (other.m_lon);
  
  Radians dlat = (tlat < olat) ? olat - tlat : tlat - olat;
  Radians dlon = (tlon < olon) ? olon - tlon : tlon - olon;
  
  Radians sdlat2 = qSin (dlat / 2);
  Radians sdlon2 = qSin (dlon / 2);
  Radians ctlat  = qCos (tlat);
  Radians colat  = qCos (olat);
  Radians root   = qSqrt (sdlat2*sdlat2 + ctlat*colat * sdlon2*sdlon2);
  
  return (2*qAsin (root));
}


Classification::Airport::DistanceInKM
Classification::Airport::distanceInKM (const Airport &other) const
{
  return (earthRadiusInKM() * distance(other));
}



Classification::Airport::DistanceInKM
Classification::Airport::earthRadiusInKM ()
{
  return (6371);
}



Classification::Airport::DistanceInKM
Classification::Airport::longestPossibleDistanceInKM ()
{
  return (M_PI * earthRadiusInKM() + 1);
}



Classification::Airport::Degrees
Classification::Airport::latitude () const
{
  return (m_lat);
}



Classification::Airport::Degrees
Classification::Airport::longitude () const
{
  return (m_lon);
}



QJsonObject
Classification::Airport::toJson () const
{
  QJsonObject obj = IcaoRecord::toJson ();

  obj.insert ("lat", m_lat);
  obj.insert ("lon", m_lon);

  return (obj);
}
