#pragma once

#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QDialog>

#include "AirlineModel.hpp"

struct EditAirlineDialog : QDialog
{
  EditAirlineDialog (QWidget * = Q_NULLPTR, Qt::WindowFlags = Qt::WindowFlags());
  virtual ~EditAirlineDialog ();
  
  void setDefinitions (QSharedPointer<Definitions>);
  
public slots:
  void selectDisplayAirline (QString);
  void createAirline ();
  void removeAirline ();
  void updateChanges ();
  void revertChanges ();
  
protected:
  QComboBox   *m_icao;
  QLineEdit   *m_name, *m_area;
  QTextEdit   *m_comment;
  
  QSharedPointer<Definitions> m_definitions;
  
  Q_OBJECT;
};
