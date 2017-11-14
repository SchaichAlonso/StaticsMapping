#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QTableView>

#include <Common/Classification/Definitions.hpp>

struct AircraftScreen : QDialog
{
  AircraftScreen (Classification::DefinitionsPointer, QWidget * =Q_NULLPTR, Qt::WindowFlags =Qt::WindowFlags());
  ~AircraftScreen ();
  
public Q_SLOTS:
  void add ();
  void drop ();
  void selectedIndexChanged (const QModelIndex &, const QModelIndex &);
  
protected:
  void createWidgets ();
  
  Classification::DefinitionsPointer m_definitions;
  QTableView *m_table;
};
