#pragma once

#include <QtCore/QStringList>
#include <QtWidgets/QWizard>

#include <Classification/Definitions.hpp>

struct Wizard : QWizard
{
  enum Pages
  {
    Introduction,
    Scan,
    Afiliates
  };
  
  
  Wizard(QString dst, QString src, QString use, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
  ~Wizard();
  
  void setDefinitions(QString prefix, Classification::DefinitionsPointer);
  void setScanSettings(QString dst, QString src, int threads, int texlimit);
  
protected:
  static QStringList airportsByAirlinePresence(Classification::DefinitionsPointer defs, bool present);
  static QStringList airlinesByObjectAvailablility(Classification::DefinitionsPointer defs);
  void showStatistics(Classification::DefinitionsPointer);
};
