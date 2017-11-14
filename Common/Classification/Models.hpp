#include "AircraftModel.hpp"
#include "AircraftModelDisplay.hpp"
#include "AirlineModel.hpp"
#include "AirlineModelDisplay.hpp"
#include "ObjectModel.hpp"

namespace Classification
{
  struct Models : QObject
  {
    Models (Definitions *);
   ~Models ();
    
    AircraftModel        *aircraft_read_write;
    AircraftModelDisplay *aircraft_read_only;
    
    AirlineModel         *airline_read_write;
    AirlineModelDisplay  *airline_read_only;
      
    ObjectModel          *object_read_write;
  };
}
