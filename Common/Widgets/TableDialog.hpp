#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QTableView>

#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/Model.hpp>

struct TableDialog : QDialog
{
  TableDialog (Classification::DefinitionsPointer, Classification::Model *, QString, QWidget * = Q_NULLPTR, Qt::WindowFlags = Qt::WindowFlags());
  virtual ~TableDialog ();
  
  virtual bool checkDuplicate (QString) const = 0;
  virtual bool verify (QString) const = 0;
  virtual int  drop (QString) = 0;
  virtual void insert (QString) = 0;
  
protected:
  void handleDropRequest ();
  void handleInsertRequest ();
  
  void createWidgets (Classification::Model *);
  
  Classification::DefinitionsPointer m_definitions;
  QString m_primary_key_name;
  QTableView *m_table;
  
  Q_OBJECT;
}; 
