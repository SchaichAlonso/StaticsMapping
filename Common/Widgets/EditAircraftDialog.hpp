#pragma once

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QDialog>

#include <QtWidgets/QDataWidgetMapper>

#include <Common/Classification/Definitions.hpp>

struct EditAircraftDialog : QDialog
{
  EditAircraftDialog (Classification::DefinitionsPointer, QWidget * = Q_NULLPTR, Qt::WindowFlags = Qt::WindowFlags());
  virtual ~EditAircraftDialog ();
  
public slots:
  void create ();
  void remove ();
  
protected:
  Classification::AircraftModel *m_data_model;
  QDataWidgetMapper             *m_data_mapper;
  
  
  QComboBox      *m_icao;
  QLineEdit      *m_name;
  QComboBox      *m_engine;
  QDoubleSpinBox *m_span, *m_mtow;
  
  Classification::DefinitionsPointer m_definitions;
  
  Q_OBJECT;
};
