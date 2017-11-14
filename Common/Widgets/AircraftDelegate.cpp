#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>

#include <Common/Classification/Aircraft.hpp>

#include "AircraftDelegate.hpp"

AircraftDelegate::AircraftDelegate (Classification::DefinitionsPointer d)
  : ListDelegate ()
  , m_definitions (d)
{
}



AircraftDelegate::~AircraftDelegate ()
{
}


QString
AircraftDelegate::displayString (const QModelIndex &index) const
{
  QString icao;
  QString name;
  Classification::AircraftPointer airline;
  
  icao = index.data(Qt::UserRole).toString();
  airline = m_definitions->aircraft (icao);
  
  if (airline) {
    name = QString("%1 (%0)").arg(airline->name()).arg(airline->icao());
  } else {
    name = "<NULLPTR>";
  }
  
  return (name);
}

