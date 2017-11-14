#include <Classification/Airline.hpp>
#include <Classification/AirlineModel.hpp>

#include "AirlineTableDialog.hpp"


AirlineTableDialog::AirlineTableDialog (
    Classification::DefinitionsPointer d,
    QWidget *parent,
    Qt::WindowFlags flags)
  : TableDialog (d, d->airlineModel(), "Airline ICAO", parent, flags)
{
}



AirlineTableDialog::~AirlineTableDialog ()
{
}



bool
AirlineTableDialog::verify (QString icao) const
{
  return (Classification::Airline().verifyIcaoCode(icao));
}



bool
AirlineTableDialog::checkDuplicate (QString key) const
{
  return (m_definitions->airline(key).isNull());
}


void
AirlineTableDialog::insert (QString key)
{
  m_definitions->upsert (
      Classification::AirlinePointer (
          new Classification::Airline (key)
      )
  );
}


int
AirlineTableDialog::drop (QString key)
{
  Classification::AirlinePointer ptr = m_definitions->airline (key);
  return (m_definitions->drop (ptr));
}
