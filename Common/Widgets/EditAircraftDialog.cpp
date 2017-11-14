#include <QFormLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include <Common/Classification/Aircraft.hpp>
#include <Common/Classification/AircraftModel.hpp>
#include <Common/Classification/Constants.hpp>

#include "AircraftDelegate.hpp"
#include "EditAircraftDialog.hpp"



EditAircraftDialog::EditAircraftDialog (Classification::DefinitionsPointer d,
                                        QWidget *parent,
                                        Qt::WindowFlags flags)
  : QDialog (parent, flags)
  , m_data_model (d->aircraftModel ())
  , m_data_mapper (new QDataWidgetMapper(this))
  , m_definitions (d)
{
  QVBoxLayout *vbox;
  QHBoxLayout *bottom;
  QFormLayout *form;
  QPushButton *create, *remove;
  
  vbox   = new QVBoxLayout ();
  bottom = new QHBoxLayout ();
  form   = new QFormLayout ();
  
  
  create = new QPushButton ("new");
  remove = new QPushButton ("delete");
  m_icao = new QComboBox ();
  m_name = new QLineEdit ();
  m_engine = new QComboBox ();
  m_span = new QDoubleSpinBox ();
  m_mtow = new QDoubleSpinBox ();
  
  m_icao->setEditable (false);
  m_span->setRange (0, 100);
  m_span->setSuffix ("m");
  m_mtow->setRange (0, 1000);
  m_mtow->setSuffix ("t");
  
  m_engine->addItems (
    QStringList ()
      << Classification::Constants::engine(Classification::Aircraft::Jet)
      << Classification::Constants::engine(Classification::Aircraft::Turbofan)
      << Classification::Constants::engine(Classification::Aircraft::Turboprop)
      << Classification::Constants::engine(Classification::Aircraft::Piston)
      << Classification::Constants::engine(Classification::Aircraft::Helicopter)
  );
  
  
  m_icao->setModel (m_data_model);
  m_icao->setItemDelegate (new AircraftDelegate (m_definitions));
  
  m_data_mapper->setModel (m_data_model);
  m_data_mapper->addMapping (m_icao,   Classification::AircraftModel::Icao, "currentText");
  m_data_mapper->addMapping (m_span,   Classification::AircraftModel::Span);
  m_data_mapper->addMapping (m_name,   Classification::AircraftModel::Name);
  m_data_mapper->addMapping (m_engine, Classification::AircraftModel::Engine, "currentText");
  m_data_mapper->addMapping (m_mtow,   Classification::AircraftModel::Mtow);
  
  form->addRow ("ICAO", m_icao);
  form->addRow ("Name", m_name);
  form->addRow ("Equipment", m_engine);
  form->addRow ("Span", m_span);
  form->addRow ("MTOW", m_mtow);
  
  bottom->addWidget (create);
  bottom->addStretch ();
  bottom->addWidget (remove);
  
  vbox->addLayout (form);
  vbox->addLayout (bottom);
  
  connect (m_icao, SIGNAL(currentIndexChanged(int)),
           m_data_mapper, SLOT(setCurrentIndex(int)));
  
  connect (create, SIGNAL(released()), this, SLOT(create()));
  connect (remove, SIGNAL(released()), this, SLOT(remove()));
  
  setLayout (vbox);
}



EditAircraftDialog::~EditAircraftDialog ()
{
}



void
EditAircraftDialog::create ()
{
  Classification::Aircraft aircraft;
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
    
    ok = aircraft.verifyIcaoCode (icao);
    if (not ok) {
      QMessageBox::warning (
          this,
          "ICAO Code",
          "Invalid ICAO Aircraft Code.");
      continue;
    }
    
    ok = not m_definitions->aircraft (icao);
    if (ok) {
      m_definitions->upsert (
        Classification::AircraftPointer(
          new Classification::Aircraft(icao)
        )
      );
    } else {
      QMessageBox::warning (
          this,
          "ICAO Code",
          QString("Aircraft %1 already exists.").arg(icao));
    }
  } while (not ok);
}



void
EditAircraftDialog::remove ()
{
  QMessageBox::StandardButton input;
  
  
  int index = m_data_mapper->currentIndex();
  QModelIndex mi = m_data_model->index(index, 0, QModelIndex());
  QString icao = m_data_model->data(mi, Qt::UserRole).toString();
  
  input = QMessageBox::question (
      this,
      "Confirm",
      "Deleting the aircraft will purge all associated objects. Continue?");
  
  if (input == QMessageBox::Yes) {
    m_definitions->drop (m_definitions->airline(icao));
  }
}
