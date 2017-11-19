add_library (Classification STATIC
  Afiliates.cpp
  Aircraft.cpp
  AircraftModel.cpp
  Airline.cpp
  AirlineModel.cpp
  Airport.cpp
  Constants.cpp
  EngineModel.cpp
  Definitions.cpp
  IcaoRecord.cpp
  Library.cpp
  LibraryModel.cpp
  Model.cpp
  Object.cpp
  ObjectModel.cpp
  Record.cpp
  WeightedObject.cpp
  XPClass.cpp
  XPLibrary.cpp
)
  

target_link_libraries (Classification
  Common
  Obj8
  Qt5::Core
)
