#include <QtWidgets/QMessageBox>

#include "AfiliatesPage.hpp"
#include "IntroPage.hpp"
#include "ScanPage.hpp"
#include "Wizard.hpp"

Wizard::Wizard (QString dst, QString src, QString use, QWidget *parent, Qt::WindowFlags flags)
  : QWizard(parent, flags)
{
  setPage (Introduction, new IntroPage(dst,src,use));
  setStartId (Introduction);
}


Wizard::~Wizard ()
{
  
}



void
Wizard::setDefinitions(QString prefix, Classification::DefinitionsPointer definitions)
{
  showStatistics (definitions);
  setPage (Afiliates, new AfiliatesPage(prefix, definitions));
}



void
Wizard::setScanSettings(QString dst, QString src, int threads, int texlimit)
{
  setPage (Scan, new ScanPage(dst, src, threads, texlimit));
}



QStringList
Wizard::airportsByAirlinePresence (Classification::DefinitionsPointer defs, bool present)
{
  Classification::Definitions::AirlinesAtAirports a(defs->airlinesAtAirports());
  QMultiMap<int, QString> highscore;
  
  Q_FOREACH (QString icao, a.uniqueKeys()) {
    if ((bool)defs->airport(icao) == present) {
      highscore.insert(-a.values(icao).size(), icao);
    }
  }
  
  return (highscore.values());
}



QStringList
Wizard::airlinesByObjectAvailablility (Classification::DefinitionsPointer defs)
{
  QMultiMap<int, QString> highscore;
  
  QMap<QString, int> counts;
  Q_FOREACH (Classification::ObjectPointer obj, defs->objects()) {
    if (not defs->airline(obj->livery())->hasFictiveIcaoCode()) {
      counts[obj->livery()]++;
    }
  }
  
  for(QMap<QString,int>::ConstIterator i=counts.constBegin(); i!=counts.constEnd(); ++i) {
    highscore.insert(-i.value(), i.key());
  }
  
  return (highscore.values());
}


void
Wizard::showStatistics (Classification::DefinitionsPointer defs)
{
  int highscore_size = 3;
  QMessageBox::information (
    this,
    "Report",
    QString(
      "Import Success.\n"
      "Objects: %1\n"
      "Top Objects Airlines: %2\n"
      "Top populated Airports: %3\n"
      "Most missing Airports: %4"
    )
    .arg(defs->objects().size())
    .arg(airlinesByObjectAvailablility(defs).mid(0, highscore_size).join(", "))
    .arg(airportsByAirlinePresence(defs,true).mid(0, highscore_size).join(", "))
    .arg(airportsByAirlinePresence(defs,false).mid(0, highscore_size).join(", "))
  );
}

