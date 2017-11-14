#include <Common/Classification/Aircraft.hpp>
#include <Common/Classification/Airline.hpp>
#include <Common/Classification/Airport.hpp>
#include <Common/Classification/Library.hpp>
#include <Common/Classification/Object.hpp>
#include <Common/CryptoHash.hpp>

#include "DefinitionsHelper.hpp"



Classification::Object::Hash
Classification::Test::DefinitionsHelper::hash (int i)
{
  QByteArray ba ((const char *)(&i), sizeof(i));
  
  ba = ba.leftJustified (CryptoHash::resultLength(), 0);
  
  return (ba.toHex());
}



Classification::Object::PrimaryKey
Classification::Test::DefinitionsHelper::objkey (int i)
{
  Object::Hash h = hash (i);
  return (Object (1, h, h).primaryKey());
}



Classification::ObjectPointer
Classification::Test::DefinitionsHelper::object (int i) const
{
  Object::PrimaryKey k = objkey (i);
  ObjectPointer retval = m_definitions->object (k);
  return (retval);
}



void
Classification::Test::DefinitionsHelper::addAircraft (QString icao)
{
  if (m_definitions->aircraft(icao).isNull()) {
    m_definitions->upsert (
        AircraftPointer (
            new Aircraft (icao)
        )
    );
  }
}



void
Classification::Test::DefinitionsHelper::addAirline (QString icao)
{
  if (m_definitions->airline(icao).isNull()) {
    m_definitions->upsert (
        AirlinePointer (
            new Airline (icao)
        )
    );
  }
}



void
Classification::Test::DefinitionsHelper::addAirport (QString icao, double lat, double lon)
{
  if (m_definitions->airport(icao).isNull()) {
    m_definitions->upsert (
        AirportPointer (
            new Airport (icao, icao, lat, lon)
        )
    );
  }
}



void
Classification::Test::DefinitionsHelper::addLibrary (QString key)
{
  if (m_definitions->library(key).isNull()) {
    m_definitions->upsert (
        LibraryPointer (
            new Library (key)
        )
    );
  }
}



void
Classification::Test::DefinitionsHelper::addObject (
    QString livery,
    QString aircraft,
    QString library,
    int id,
    bool auto_add
)
{
  if (auto_add) {
    addAircraft (aircraft);
    addAirline (livery);
    addLibrary (library);
  }
  
  ObjectPointer obj (new Object (1, hash(id), hash(id)));
  obj->setLivery (livery);
  obj->setAircraft (aircraft);
  obj->setLibrary (library);
  obj->setPurpose (Object::Airliner);
  
  m_definitions->upsert (obj);
}



void
Classification::Test::DefinitionsHelper::init ()
{
  m_definitions.reset (new Definitions ());
}



void
Classification::Test::DefinitionsHelper::cleanup ()
{
  m_definitions.reset ();
}
