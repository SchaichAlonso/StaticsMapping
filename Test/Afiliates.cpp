#include <Classification/Afiliates.hpp>

#include <Classification/Aircraft.hpp>
#include <Classification/Airline.hpp>
#include <Classification/Airport.hpp>

#include "Afiliates.hpp"


Classification::WeightedObject::Weight
Classification::Test::Afiliates::weight (QList<WeightedObject> list, ObjectPointer object)
{
  Q_FOREACH (const Classification::WeightedObject &candidate, list) {
    if (candidate.object == object) {
      return (candidate.weight);
    }
  }
  
  return (-1);
}


void
Classification::Test::Afiliates::init ()
{
  DefinitionsHelper::init ();
  
  addAircraft("E135");
  addAircraft("E75L");
  addAircraft("SU95");
  addAircraft("AT75");
  
  m_definitions->aircraft("E135")->setWingspan(20040);
  m_definitions->aircraft("E135")->setEngine(Aircraft::Turbofan);
  m_definitions->aircraft("E75L")->setWingspan(28650);
  m_definitions->aircraft("E75L")->setEngine(Aircraft::Turbofan);
  m_definitions->aircraft("SU95")->setWingspan(27800);
  m_definitions->aircraft("SU95")->setEngine(Aircraft::Turbofan);
  m_definitions->aircraft("AT75")->setWingspan(27050);
  m_definitions->aircraft("AT75")->setEngine(Aircraft::Turboprop);
  
  addAirline ("AFL");
  addAirline ("FFM");
  addAirline ("MAS");
  m_definitions->airline("FFM")->setParent("MAS");
  
  addAirport ("LFPO",  48.723333,    2.379444);
  addAirport ("UUEE",  55.972778,   37.414722);
  addAirport ("VHHH",  22.308889,  113.914444);
  addAirport ("WMKK",   2.743333,  101.698056);
  addAirport ("WMKP",   5.297139,  100.276864);
  addAirport ("WMSA",   3.131111,  101.548056);
  
  m_definitions->airline("AFL")->setHubs ("UUEE");
  m_definitions->airline("FFM")->setHubs ("WMSA WMKP");
  m_definitions->airline("MAS")->setHubs ("WMKK");
  
  addLibrary("x");
}


void
Classification::Test::Afiliates::basic ()
{
  addObject ("FFM", "AT75", "x", 1, false);
  
  Classification::Afiliations a (m_definitions);
  WeightedObjectsByXPClass afl = a.objectsAvailable("AFL");
  WeightedObjectsByXPClass ffm = a.objectsAvailable("FFM");
  WeightedObjectsByXPClass mas = a.objectsAvailable("MAS");
  
  XPClass at75 (Object::Airliner, Aircraft::Turboprop, Aircraft::SizeC);
  
  QVERIFY (afl.isEmpty());
  QVERIFY (ffm.contains(at75));
  QVERIFY (mas.contains(at75));
}



void
Classification::Test::Afiliates::sizing ()
{
  addObject ("FFM", "AT75", "x", 1, false);
  addObject ("FFM", "E135", "x", 2, false);
  addObject ("FFM", "E75L", "x", 3, false);
  addObject ("FFM", "SU95", "x", 4, false);
  
  ObjectPointer at75 = object(1);
  ObjectPointer e135 = object(2);
  ObjectPointer e75l = object(3);
  ObjectPointer su95 = object(4);
  
  
  
  Classification::Afiliations a (m_definitions);
  WeightedObjectsByXPClass ffm = a.objectsAvailable("FFM");
  WeightedObjectsByXPClass mas = a.objectsAvailable("MAS");
  
  
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turboprop, Aircraft::SizeA)).size() == 0);
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turboprop, Aircraft::SizeB)).size() == 0);
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turboprop, Aircraft::SizeC)).size() == 1);
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turboprop, Aircraft::SizeD)).size() == 1);
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turboprop, Aircraft::SizeE)).size() == 1);
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turboprop, Aircraft::SizeF)).size() == 1);
  
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turboprop, Aircraft::SizeC)), at75) >= 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turboprop, Aircraft::SizeD)), at75) > 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turboprop, Aircraft::SizeE)), at75) > 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turboprop, Aircraft::SizeF)), at75) > 0);
  
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeA)).size() == 0);
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeB)).size() == 1);
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeC)).size() == 2);
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeD)).size() == 2);
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeE)).size() == 2);
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)).size() == 2);
  
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeB)), e135) >= 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeC)), e75l) >= 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeC)), su95) >= 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeD)), e75l) > 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeD)), su95) > 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeE)), e75l) > 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeE)), su95) > 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)), e75l) > 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)), su95) > 0);
  
  QVERIFY (mas.size() == ffm.size());
  for (WeightedObjectsByXPClass::ConstIterator i = ffm.constBegin(); i!=ffm.constEnd(); ++i) {
    QVERIFY (mas.contains (i.key()));
    QVERIFY (weight(mas.values(i.key()), i.value().object) > i.value().weight);
  }
}



void
Classification::Test::Afiliates::explicitAge ()
{
  addObject ("FFM", "E135", "x", 1, false);
  addObject ("FFM", "E135", "x", 2, false);
  addObject ("FFM", "E135", "x", 3, false);
  
  object(1)->setIntroduced(1990);
  object(1)->setRetired(1993);
  
  object(2)->setIntroduced(1990);
  object(2)->setRetired(2000);
  
  object(3)->setIntroduced(1997);
  object(3)->setRetired(2010);
  
  Classification::Afiliations a (m_definitions);
  
  a.setYear (1995);
  
  WeightedObjectsByXPClass ffm = a.objectsAvailable("FFM");
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)).size() == 1);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)), object(2)) >= 0);
}



void
Classification::Test::Afiliates::implicitAge ()
{
  addObject ("FFM", "E75L", "x", 1, false);
  addObject ("FFM", "SU95", "x", 2, false);
  m_definitions->aircraft("E75L")->setIntroduced (2002);
  m_definitions->aircraft("SU95")->setIntroduced (2011);
  
  Classification::Afiliations a (m_definitions);
  
  a.setYear (2005);
  
  WeightedObjectsByXPClass ffm, mas;
  
  ffm = a.objectsAvailable("FFM");
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)).size() == 1);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)), object(1)) >= 0);
  
  m_definitions->airline("FFM")->setFounded (2007);
  m_definitions->airline("MAS")->setFounded (1947);
  
  ffm = a.objectsAvailable("FFM");
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)).size() == 0);
  
  /*
   * it's FFM's object, so MAS doesn't get it even though MAS is old enough.
   */
  mas = a.objectsAvailable("FFM");
  QVERIFY (mas.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)).size() == 0);
  
  
  
  
  m_definitions->airline("MAS")->setCeased (2010);
  
  a.setYear (2011);
  ffm = a.objectsAvailable("FFM");
  mas = a.objectsAvailable("MAS");
  
  QVERIFY (ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)).size() == 2);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)), object(1)) >= 0);
  QVERIFY (weight(ffm.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)), object(2)) >= 0);
  QVERIFY (mas.values(XPClass(Object::Airliner, Aircraft::Turbofan, Aircraft::SizeF)).size() == 0);
}


QTEST_MAIN (Classification::Test::Afiliates);
