#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextEdit>

#include "AirportDataQueryDialog.hpp"

namespace Widgets
{
  AirportDataQueryDialog::AirportDataQueryDialog(
    Classification::DefinitionsPointer definitions,
    QWidget* parent,
    Qt::WindowFlags f
  )
  : QDialog(parent, f)
  , m_sync(definitions, definitions->airlinesAtAirports().uniqueKeys())
  {
    QDialogButtonBox *buttons(new QDialogButtonBox(QDialogButtonBox::Close));
    QTextEdit *report(new QTextEdit());
    QVBoxLayout *layout(new QVBoxLayout());
    
    layout->addWidget(report);
    layout->addWidget(buttons);
    setLayout(layout);
    
    buttons->setEnabled(false);
    
    connect(
      &m_sync, &Classification::AirportDataQuery::synced,
      [report](QString icao) {
        report->append(tr("%1 synced successfuly").arg(icao));
      }
    );
    
    connect(
      &m_sync, &Classification::AirportDataQuery::skipped,
      [report](QString icao) {
        report->append(tr("%1 skipped").arg(icao));
      }
    );
    
    connect(
      &m_sync, &Classification::AirportDataQuery::failed,
      [report](QString icao) {
        report->append(tr("%1 failed").arg(icao));
      }
    );
    
    connect(
      &m_sync, &Classification::AirportDataQuery::completed,
      [buttons,report]() {
        report->append(tr("completed"));
        buttons->setEnabled(true);
      }
    );
    
    connect(
      buttons, &QDialogButtonBox::accepted,
      this, &QDialog::accept
    );
    connect(
      buttons, &QDialogButtonBox::rejected,
      this, &QDialog::accept
    );
  }

  AirportDataQueryDialog::~AirportDataQueryDialog()
  {
  }
  
  
  int AirportDataQueryDialog::exec()
  {
    m_sync.request();
    return (QDialog::exec());
  }
}
