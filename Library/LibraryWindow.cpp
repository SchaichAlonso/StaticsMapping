#include <QtWidgets/QApplication>

#include <Common/Classification/AircraftModel.hpp>
#include <Common/Classification/AirlineModel.hpp>

#include <Common/Widgets/GlobalDistributionDialog.hpp>

#include "LibraryWindow.hpp"
#include "ObjViewDialog.hpp"
#include "PreviewablesModel.hpp"

#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>


PropertiesPage::PropertiesPage (Classification::DefinitionsPointer definitions, QWidget *parent)
  : QWidget (parent)
  , m_definitions ()
  , m_afiliations (definitions)
  , m_airline (new QComboBox)
  , m_airline_model ()
  , m_previewables (new QTableView)
  , m_previewables_model (new ObjectsByXPClassModel)
{
  createGui ();
  resetDefinitions (definitions);
}



PropertiesPage::~PropertiesPage ()
{
}



void
PropertiesPage::resetDefinitions (Classification::DefinitionsPointer definitions)
{
  m_airline_model.reset (definitions->airlineModel());
  m_afiliations = Classification::Afiliations (definitions);
  
  m_airline->setModel (m_airline_model.data());
  m_airline->setModelColumn (m_airline_model->column(Classification::Airline::IcaoProperty));
  
  m_definitions = definitions;
  
  updatePreviewables();
}



void
PropertiesPage::updatePreviewables ()
{
  Classification::AirlinePointer airline;
  Classification::WeightedObjectsByXPClass objects;
  
  if (m_definitions) {
    QString icao = m_airline->currentData().toString();
    airline = m_definitions->airline(icao);
    objects = m_afiliations.objectsAvailable (airline);
  }
  
  m_previewables_model->setObjectsByXPClass (objects);
}



void
PropertiesPage::showPreviewDialog ()
{
  QModelIndex i = m_previewables->currentIndex();
  
  if (i.isValid()) {
    QString key = i.data(Qt::UserRole).toString();
    Classification::ObjectPointer o = m_definitions->object(key);
    
    if (o) {
      ObjViewDialog dialog("/usr/home/alonso/new", m_definitions, o, this);
      dialog.setWindowTitle(o->fileName());
      dialog.setModal(true);
      dialog.exec();
    }
  }
}



void
PropertiesPage::createGui ()
{
  QVBoxLayout *layout  = new QVBoxLayout ();
  
  layout->addWidget (createFiltersGui ());
  layout->addWidget (createPreviewsGui ());
  
  setLayout (layout);
}



QWidget *
PropertiesPage::createPreviewsGui ()
{
  QGroupBox   *group   = new QGroupBox ("Previews");
  QVBoxLayout *vbox    = new QVBoxLayout ();
  QGridLayout *form    = new QGridLayout ();
  QPushButton *preview = new QPushButton ("Preview Selected");
  
  m_previewables->setModel (m_previewables_model.data());
  m_previewables->setSortingEnabled (true);
  m_previewables->horizontalHeader()->setStretchLastSection(true);
  
  void (QComboBox::*current_index_changed)(int) = &QComboBox::currentIndexChanged;
  connect (
    m_airline, current_index_changed,
    this, &PropertiesPage::updatePreviewables
  );
  
  connect (
    preview, &QPushButton::released,
    this, &PropertiesPage::showPreviewDialog
  );
  
  form->addWidget (new QLabel("Operator"), 0, 0, 1, 1);
  form->addWidget (m_airline, 0, 1, 1, 1);
  form->addWidget (m_previewables, 1, 0, 1, 2);
  form->setColumnStretch(0, 0);
  form->setColumnStretch(1, 1);
  
  vbox->addLayout (form);
  vbox->addWidget (preview);
  
  group->setLayout (vbox);
  
  return (group);
}



QWidget *
PropertiesPage::createFiltersGui ()
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
        m_afiliations.setMaxPlaneAge (x);
        updatePreviewables ();
      }
  );
  
  connect (
      year, QSpinBox__valueChanged,
      this, [&](int x) {
        m_afiliations.setYear (x);
        updatePreviewables ();
      }
  );
  
  form->addRow ("Current Year:", year);
  form->addRow ("Max. Plane Age:", age);
  
  group->setLayout (form);
  
  return (group);
}


int main (int argsc, char **argsv)
{
  QApplication app(argsc, argsv);
  app.setQuitOnLastWindowClosed(true);
  
  //PropertiesPage dialog (Classification::Definitions::fromFile());
  PropertiesPage dialog (Classification::Definitions::fromFile("/usr/home/alonso/new/found.json"));
  dialog.show();
  
  return (app.exec());
}
