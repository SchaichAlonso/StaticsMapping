#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>

#include <Common/Classification/Airline.hpp>

#include "AirlineDelegate.hpp"

AirlineDelegate::AirlineDelegate (Classification::DefinitionsPointer d)
  : ListDelegate ()
  , m_definitions (d)
{
}



AirlineDelegate::~AirlineDelegate ()
{
}


QString
AirlineDelegate::displayString (const QModelIndex &index) const
{
  QString icao;
  QString name;
  Classification::AirlinePointer airline;
  
  icao = index.data(Qt::UserRole).toString();
  airline = m_definitions->airline(icao);
  
  if (airline) {
    name = QString("%1 (%0)").arg(airline->name()).arg(airline->icao());
  } else {
    name = "<NULLPTR>";
  }
  
  return (name);
}

