#include <Common/Classification/Aircraft.hpp>
#include <Common/Classification/Airline.hpp>
#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/Object.hpp>

#include "Definitions.hpp"



void
Classification::Test::Definitions::removeAirlineFirst ()
{
  addObject ("AAL", "B731", "lib", 0, true);
  addObject ("AAL", "B732", "lib", 1, true);
  addObject ("AFL", "B733", "lib", 2, true);
  addObject ("SBI", "B734", "lib", 3, true);
  addObject ("AFR", "B735", "lib", 4, true);
  
  m_definitions->drop (m_definitions->airline("AAL"));
  
  QVERIFY (not m_definitions->airline("AAL"));
  QVERIFY (m_definitions->airline("AFL"));
  QVERIFY (m_definitions->airline("SBI"));
  QVERIFY (m_definitions->airline("AFR"));
  
  QVERIFY (not m_definitions->object (objkey(0)));
  QVERIFY (not m_definitions->object (objkey(1)));
  QVERIFY (m_definitions->object (objkey(2)));
  QVERIFY (m_definitions->object (objkey(3)));
  QVERIFY (m_definitions->object (objkey(4)));
}

void
Classification::Test::Definitions::removeAirlineMiddle ()
{
  addObject ("AFL", "B738", "lib", 1, true);
  addObject ("AFL", "B738", "lib", 2, true);
  addObject ("SBI", "B738", "lib", 3, true);
  addObject ("AFR", "B738", "lib", 4, true);
  
  m_definitions->drop (m_definitions->airline("SBI"));
  
  QVERIFY (m_definitions->airline("AFL"));
  QVERIFY (not m_definitions->airline("SBI"));
  QVERIFY (m_definitions->airline("AFR"));
  
  QVERIFY (m_definitions->object (objkey(1)));
  QVERIFY (m_definitions->object (objkey(2)));
  QVERIFY (not m_definitions->object (objkey(3)));
  QVERIFY (m_definitions->object (objkey(4)));
}


void
Classification::Test::Definitions::removeAirlineLast ()
{
  addObject ("AAL", "B731", "lib", 0, true);
  addObject ("AAL", "B732", "lib", 1, true);
  addObject ("AFL", "B733", "lib", 2, true);
  addObject ("SBI", "B734", "lib", 3, true);
  addObject ("AFR", "B735", "lib", 4, true);
  
  m_definitions->drop (m_definitions->airline("AFR"));
  
  QVERIFY (m_definitions->airline("AAL"));
  QVERIFY (m_definitions->airline("AFL"));
  QVERIFY (m_definitions->airline("SBI"));
  QVERIFY (not m_definitions->airline("AFR"));
  
  QVERIFY (m_definitions->object (objkey(0)));
  QVERIFY (m_definitions->object (objkey(1)));
  QVERIFY (m_definitions->object (objkey(2)));
  QVERIFY (m_definitions->object (objkey(3)));
  QVERIFY (not m_definitions->object (objkey(4)));
}


void
Classification::Test::Definitions::removeAddAirline ()
{
  addObject ("AFL", "B738", "lib", 1, true);
  addObject ("AFL", "B738", "lib", 2, true);
  addObject ("SBI", "B738", "lib", 3, true);
  addObject ("AFR", "B738", "lib", 4, true);
  
  m_definitions->drop (m_definitions->airline("SBI"));
  
  /*
   * covered by removeMiddle
   */
  
  addObject ("AFR", "B738", "lib", 3, true);
  addObject ("SBI", "B738", "lib", 5, true);
  
  QVERIFY (m_definitions->object (objkey(1)));
  QVERIFY (m_definitions->object (objkey(2)));
  QVERIFY (m_definitions->object (objkey(3)));
  QVERIFY (m_definitions->object (objkey(4)));
  QVERIFY (m_definitions->object (objkey(5)));
}


QTEST_MAIN (Classification::Test::Definitions);
