#include <Classification/Aircraft.hpp>
#include <Classification/AircraftModel.hpp>

#include "AircraftTableDialog.hpp"

namespace Widgets
{
  AircraftTableDialog::AircraftTableDialog (
    Classification::DefinitionsPointer d,
    QWidget *parent,
    Qt::WindowFlags flags
  )
  : TableDialog(d, d->aircraftModel(), "Aircraft ICAO", parent, flags)
  {
  }
  
  
  
  AircraftTableDialog::~AircraftTableDialog ()
  {
  }
  
  
  
  bool
  AircraftTableDialog::verify (QString icao) const
  {
    return (Classification::Aircraft().verifyIcaoCode(icao));
  }
  
  
  
  bool
  AircraftTableDialog::checkDuplicate (QString key) const
  {
    return (m_definitions->aircraft(key).isNull());
  }
  
  
  void
  AircraftTableDialog::insert (QString key)
  {
    m_definitions->upsert (
      Classification::AircraftPointer(new Classification::Aircraft(key))
    );
  }
  
  
  int
  AircraftTableDialog::drop (QString key)
  {
    Classification::AircraftPointer ptr = m_definitions->aircraft (key);
    return (m_definitions->drop (ptr));
  }
}
