#include <QtWidgets/QAction>

#include <Classification/Airline.hpp>
#include <Classification/AirlineModel.hpp>
#include <Widgets/AirportDataQueryDialog.hpp>

#include "AirlineTableDialog.hpp"

namespace Widgets
{
  AirlineTableDialog::AirlineTableDialog(
    Classification::DefinitionsPointer d,
    QWidget *parent,
    Qt::WindowFlags flags
  )
  : TableDialog(d, d->airlineModel(), "Airline ICAO", parent, flags)
  {
    extendToolbar();
  }
  
  
  
  AirlineTableDialog::~AirlineTableDialog()
  {
  }
  
  
  
  bool
  AirlineTableDialog::verify(QString icao) const
  {
    return (Classification::Airline().verifyIcaoCode(icao));
  }
  
  
  
  bool
  AirlineTableDialog::checkDuplicate(QString key) const
  {
    return (m_definitions->airline(key).isNull());
  }
  
  
  void
  AirlineTableDialog::insert(QString key)
  {
    m_definitions->upsert (
     Classification::AirlinePointer(new Classification::Airline(key))
    );
  }
  
  
  int
  AirlineTableDialog::drop(QString key)
  {
    Classification::AirlinePointer ptr = m_definitions->airline (key);
    return (m_definitions->drop (ptr));
  }
  
  
  void
  AirlineTableDialog::extendToolbar()
  {
    m_toolbar->addSeparator();
    QAction *queries(m_toolbar->addAction(tr("Query Missing Airports")));
    QAction *save(m_toolbar->addAction(tr("Save Definitions")));
    
    connect(
      queries, &QAction::triggered,
      this, &AirlineTableDialog::queryMissingAirports
    );
    
    connect(
      save, &QAction::triggered,
      this, &AirlineTableDialog::saveDefinitions
    );
  }
  
  
  void
  AirlineTableDialog::queryMissingAirports()
  {
    AirportDataQueryDialog(m_definitions, this).exec();
  }
  
  
  void
  AirlineTableDialog::saveDefinitions()
  {
    m_definitions->toFile();
  }
}
