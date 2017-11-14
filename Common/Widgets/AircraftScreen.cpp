#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>

#include <Classification/Aircraft.hpp>
#include <Classification/AircraftModel.hpp>

#include "AircraftScreen.hpp"

AircraftScreen::AircraftScreen (Classification::DefinitionsPointer d, QWidget *p, Qt::WindowFlags f)
  : QDialog (p, f)
  , m_definitions (d)
{
  createWidgets ();
}



AircraftScreen::~AircraftScreen ()
{
}



void
AircraftScreen::createWidgets ()
{
  QVBoxLayout *layout = new QVBoxLayout (this);
  QToolBar    *toolbar = new QToolBar ();
  QAction     *add, *drop, *close;
  
  m_table = new QTableView ();
  
  m_table->setModel (m_definitions->aircraftModel ());
  
  add    = toolbar->addAction ("Create");
  drop   = toolbar->addAction ("Delete Selected");
  close  = toolbar->addAction ("Close Window");
  
  layout->addWidget (toolbar);
  layout->addWidget (m_table);
  
  connect (add, &QAction::triggered,
           this, &AircraftScreen::add);
  connect (drop, &QAction::triggered,
           this, &AircraftScreen::drop);
  connect (close, &QAction::triggered,
           this, &AircraftScreen::accepted);
}



void
AircraftScreen::add ()
{
  Classification::AircraftPointer a(new Classification::Aircraft);
  QString icao;
  bool ok;
  
  do {
    icao = QInputDialog::getText (
        this,
        "ICAO Code",
        "ICAO Aircraft Identifier:",
        QLineEdit::Normal,
        "B738",
        &ok);
    
    if (not ok) {
      return;
    }
    
    ok = a->verifyIcaoCode (icao);
    if (not ok) {
      QMessageBox::warning (
          this,
          "ICAO Code",
          "Invalid ICAO Aircraft Code.");
      continue;
    }
    
    ok = not m_definitions->aircraft (icao);
    if (ok) {
      a->setIcao (icao.toUpper ());
      m_definitions->upsert (a);
    } else {
      QMessageBox::warning (
          this,
          "ICAO Code",
          QString("Aircraft %1 already exists.").arg(icao));
    }
  } while (not ok);
}



void
AircraftScreen::drop ()
{
  QModelIndex mi = m_table->currentIndex();
  
  if (mi.isValid()) {
    
    QString key = mi.data(Qt::UserRole).toString();
    
    QMessageBox::StandardButton input;
    input = QMessageBox::question (
        this,
        "Confirm",
        tr("Deleting the aircraft %1 will purge all associated objects. Continue?")
            .arg(key)
    );
    
    if (input == QMessageBox::Yes) {
      int n = m_definitions->purgeAircraft (key);
      QMessageBox::information (
          this,
          tr("Report"),
          tr("%1 Entities were removed.")
              .arg(n)
      );
    }
  }
}
