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
  setPage (Afiliates, new AfiliatesPage(prefix, definitions));
}



void
Wizard::setScanSettings(QString dst, QString src, int threads, int texlimit)
{
  setPage (Scan, new ScanPage(dst, src, threads, texlimit));
}

