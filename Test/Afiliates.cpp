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
  
  E135 = addAircraft("E135");
  E75L = addAircraft("E75L");
  SU95 = addAircraft("SU95");
  AT75 = addAircraft("AT75");
  
  E135->setIntroduced(1995);
  E135->setWingspan(20040);
  E135->setEngine(Aircraft::Turbofan);
  E75L->setIntroduced(2002);
  E75L->setWingspan(28650);
  E75L->setEngine(Aircraft::Turbofan);
  SU95->setIntroduced(2011);
  SU95->setWingspan(27800);
  SU95->setEngine(Aircraft::Turbofan);
  AT75->setIntroduced(1988);
  AT75->setWingspan(27050);
  AT75->setEngine(Aircraft::Turboprop);
  
  
  addAirport ("LFPO",  48.723333,    2.379444);
  addAirport ("UUEE",  55.972778,   37.414722);
  addAirport ("ULLI",  59.800278,   30.262500);
  addAirport ("UUWW",  55.596111,   37.267500);
  addAirport ("VHHH",  22.308889,  113.914444);
  addAirport ("WMKK",   2.743333,  101.698056);
  addAirport ("WMKP",   5.297139,  100.276864);
  addAirport ("WMSA",   3.131111,  101.548056);
  
  FFM = addAirline ("FFM", QStringList() << "WMSA" << "WMKP");
  MAS = addAirline ("MAS", QStringList() << "WMKK");
  FFM->setParent(MAS->icao());
  FFM->setFounded(2007);
  MAS->setFounded(1947);
  
  AFL = addAirline("AFL", QStringList() << "UUEE");
  PLK = addAirline("PLK", QStringList() << "ULLI");
  SDM = addAirline("SDM", QStringList() << "ULLI" << "UUWW");
  AFL->setFounded(1923);
  PLK->setFounded(1932);
  PLK->setCeased(2006);
  SDM->setFounded(1992);
  SDM->setParent(AFL->icao());
  PLK->setParent(SDM->icao());
  
  addLibrary("x");
}


void
Classification::Test::Afiliates::regional ()
{
  AirlinePointer AFR = addAirline("AFR", QStringList() << "LFPO");
  AirlinePointer CPA = addAirline("CPA", QStringList() << "VHHH");
  AirlinePointer ZZZ = addAirline("ZZZ");
  
  ObjectPointer a = addObject (AFL->icao(), AT75->icao(), "x", 1, false);
  ObjectPointer f = addObject (FFM->icao(), AT75->icao(), "x", 2, false);
  
  Classification::Afiliations afiliates (m_definitions);
  afiliates.setRegionSize(1);
  
  WeightedObjectsByXPClass afr = afiliates.objectsAvailable(AFR);
  WeightedObjectsByXPClass cpa = afiliates.objectsAvailable(CPA);
  WeightedObjectsByXPClass zzz = afiliates.objectsAvailable(ZZZ);
  
  XPClass at75 (Object::Airliner, Aircraft::Turboprop, Aircraft::SizeC);
  
  QVERIFY (weight(afr.values(at75), a) >=  0);
  QVERIFY (weight(afr.values(at75), f) == -1);
  
  QVERIFY (weight(cpa.values(at75), a) == -1);
  QVERIFY (weight(cpa.values(at75), f) >=  0);
  
  QVERIFY (weight(zzz.values(at75), a) >= 0);
  QVERIFY (weight(zzz.values(at75), f) >= 0);
}



void
Classification::Test::Afiliates::relatives ()
{
  ObjectPointer a = addObject (AFL->icao(), AT75->icao(), "x", 1, false);
  ObjectPointer f = addObject (FFM->icao(), AT75->icao(), "x", 2, false);
  
  Classification::Afiliations afiliates (m_definitions);
  afiliates.setRegionSize(1000000);
  
  WeightedObjectsByXPClass mas = afiliates.objectsAvailable(MAS);
  WeightedObjectsByXPClass sdm = afiliates.objectsAvailable(SDM);
  
  QVERIFY (weight(mas.values(XPClass(a->purpose(), AT75->engine(), Aircraft::SizeC)), a) == -1);
  QVERIFY (weight(mas.values(XPClass(f->purpose(), AT75->engine(), Aircraft::SizeC)), f) >=  0);
  
  QVERIFY (weight(sdm.values(XPClass(a->purpose(), AT75->engine(), Aircraft::SizeC)), a) >=  0);
  QVERIFY (weight(sdm.values(XPClass(f->purpose(), AT75->engine(), Aircraft::SizeC)), f) == -1);
}



void
Classification::Test::Afiliates::resizing ()
{
  ObjectPointer o = addObject (FFM->icao(), AT75->icao(), "x", 1, false);
  
  Classification::Afiliations a (m_definitions);
  a.setResizeWeight (1);
  
  WeightedObjectsByXPClass ffm = a.objectsAvailable("FFM");
  
  QCOMPARE (ffm.values(XPClass(o->purpose(), AT75->engine(), Aircraft::SizeA)).size(), 0);
  QCOMPARE (ffm.values(XPClass(o->purpose(), AT75->engine(), Aircraft::SizeB)).size(), 0);
  QCOMPARE (ffm.values(XPClass(o->purpose(), AT75->engine(), Aircraft::SizeC)).size(), 1);
  QCOMPARE (ffm.values(XPClass(o->purpose(), AT75->engine(), Aircraft::SizeD)).size(), 1);
  QCOMPARE (ffm.values(XPClass(o->purpose(), AT75->engine(), Aircraft::SizeE)).size(), 1);
  QCOMPARE (ffm.values(XPClass(o->purpose(), AT75->engine(), Aircraft::SizeF)).size(), 1);
  
  QCOMPARE (weight(ffm.values(XPClass(o->purpose(), AT75->engine(), Aircraft::SizeC)), o), 0);
  QCOMPARE (weight(ffm.values(XPClass(o->purpose(), AT75->engine(), Aircraft::SizeD)), o), 1);
  QCOMPARE (weight(ffm.values(XPClass(o->purpose(), AT75->engine(), Aircraft::SizeE)), o), 2);
  QCOMPARE (weight(ffm.values(XPClass(o->purpose(), AT75->engine(), Aircraft::SizeF)), o), 3);
}



void
Classification::Test::Afiliates::explicitAge ()
{
  ObjectPointer past    = addObject (MAS->icao(), E135->icao(), "x", 1, false);
  ObjectPointer current = addObject (MAS->icao(), E135->icao(), "x", 2, false);
  ObjectPointer future  = addObject (MAS->icao(), E135->icao(), "x", 3, false);
  
  past->setIntroduced(1990);
  past->setRetired(1993);
  current->setIntroduced(1990);
  current->setRetired(2000);
  future->setIntroduced(1997);
  future->setRetired(2010);
  
  Afiliations a(m_definitions);
  a.setYear(1995);
  a.setRetireAge(30);
  a.setRetireWeight(5);
  
  QCOMPARE(a.introductionDate(past, MAS, MAS), 1990);
  QCOMPARE(a.retirementDate(past, MAS, MAS), 1993);
  
  QCOMPARE(a.introductionDate(current, MAS, MAS), 1990);
  QCOMPARE(a.retirementDate(current, MAS, MAS), 2000);
  
  QCOMPARE(a.introductionDate(future, MAS, MAS), 1997);
  QCOMPARE(a.retirementDate(future, MAS, MAS), 2010);
  
  QCOMPARE(a.introduced(past, MAS, MAS), true);
  QCOMPARE(a.introduced(current, MAS, MAS), true);
  QCOMPARE(a.introduced(future, MAS, MAS), false);
  
  /*
   * ffm does not yet exist.
   */
  QCOMPARE(a.introduced(past, MAS, FFM), false);
  QCOMPARE(a.introduced(current, MAS, FFM), false);
  QCOMPARE(a.introduced(future, MAS, FFM), false);
  
  QCOMPARE(a.age(past, MAS, MAS), 10);
  QCOMPARE(a.age(current, MAS, MAS), 0);
}



void
Classification::Test::Afiliates::implicitAge ()
{
  ObjectPointer o = addObject (AFL->icao(), AT75->icao(), "x", 1, false);
  
  Afiliations a(m_definitions);
  a.setYear(1995);
  a.setRetireAge(30);
  
  QCOMPARE(a.introductionDate(o, AFL, AFL), AT75->introduced());
  QCOMPARE(a.introductionDate(o, AFL, SDM), SDM->founded());
  QCOMPARE(a.introductionDate(o, SDM, MAS), SDM->founded());
  QCOMPARE(a.introductionDate(o, SDM, SDM), SDM->founded());
  
  QCOMPARE(a.retirementDate(o, AFL, AFL), AT75->introduced() + a.retireAge());
  QCOMPARE(a.retirementDate(o, AFL, PLK), PLK->ceased());
  QCOMPARE(a.retirementDate(o, PLK, AFL), PLK->ceased());
  QCOMPARE(a.retirementDate(o, PLK, PLK), PLK->ceased());
}


QTEST_MAIN (Classification::Test::Afiliates);
