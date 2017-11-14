#include "Models.hpp"



Classification::Models::Models (Definitions *d)
  : aircraft_read_write (new AircraftModel (d, this))
  , aircraft_read_only (new AircraftModelDisplay (d, this))
  , airline_read_write (new AirlineModel (d, this))
  , airline_read_only (new AirlineModelDisplay (d, this))
  , object_read_write (new ObjectModel (d, this))
{
}



Classification::Models::~Models ()
{
  Q_ASSERT (aircraft_read_write->QObject::parent() == this);
  Q_ASSERT (aircraft_read_only->QObject::parent() == this);
  Q_ASSERT (airline_read_write->QObject::parent() == this);
  Q_ASSERT (airline_read_only->QObject::parent() == this);
  Q_ASSERT (object_read_write->QObject::parent() == this);
}
