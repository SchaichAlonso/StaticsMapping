#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>

#include <Classification/AirlineModel.hpp>

#include "AirlineMap.hpp"
#include "GlobalDistributionDialog.hpp"

GlobalDistributionDialog::GlobalDistributionDialog (
    Classification::DefinitionsPointer definitions,
    QWidget *parent,
    Qt::WindowFlags flags
)
: QDialog (parent, flags)
, m_afilia (definitions)
, m_selected_airline ()
{
  createGui (definitions);
}


GlobalDistributionDialog::~GlobalDistributionDialog ()
{
}



void
GlobalDistributionDialog::createGui (Classification::DefinitionsPointer definitions)
{
  QHBoxLayout *layout  = new QHBoxLayout ();
  QFormLayout *form    = new QFormLayout ();
  QComboBox   *airline = new QComboBox ();
  
  m_globe = new GlobalDistributionWidget (definitions);
  
  Classification::AirlineModel *mdl = definitions->airlineModel ();
  airline->setModel (mdl);
  airline->setModelColumn (mdl->column(Classification::Airline::IcaoProperty));
  
  void (QComboBox::*current_index_changed)(int) = &QComboBox::currentIndexChanged;
  connect (
     airline, current_index_changed,
     this, [=]() {
       m_selected_airline = airline->currentData().toString();
       updatePreview ();
     }
  );
  
  form->addRow ("Highlight", airline);
  
  form->addRow (createFiltersGui ());
  
  layout->addWidget (m_globe, 1);
  layout->addLayout (form);
  
  
  setLayout (layout);
}



QWidget *
GlobalDistributionDialog::createFiltersGui ()
{
  QGroupBox *group = new QGroupBox ("Filters");
  QFormLayout *form = new QFormLayout ();
  QSpinBox *age  = new QSpinBox ();
  QSpinBox *year = new QSpinBox ();
  
  year->setRange (0, 2100);
  
  void (QSpinBox::*QSpinBox__valueChanged)(int) = &QSpinBox::valueChanged;
  connect (
      age, QSpinBox__valueChanged,
      this, [&](int x) {
        m_afilia.setMaxPlaneAge (x);
        updatePreview ();
      }
  );
  
  connect (
      year, QSpinBox__valueChanged,
      this, [&](int x) {
        m_afilia.setYear (x);
        updatePreview ();
      }
  );
  
  form->addRow ("Current Year:", year);
  form->addRow ("Max. Plane Age:", age);
  
  group->setLayout (form);
  
  return (group);
}



void
GlobalDistributionDialog::updatePreview ()
{
  Classification::ObjectsByXPClass selection =
      m_afilia.objectsAvailableToAirline (m_selected_airline);
  
  GlobalDistributionWidget::ObjectList list;
  Q_FOREACH (Classification::ObjectPointer obj, selection) {
    list.append (obj);
  }
  
  m_globe->setSelectedObjects (list);
}
