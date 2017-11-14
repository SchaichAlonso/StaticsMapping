#pragma once

#include "TableDialog.hpp"

struct ObjectTableDialog : TableDialog
{
  ObjectTableDialog (Classification::DefinitionsPointer, QWidget * = Q_NULLPTR, Qt::WindowFlags = Qt::WindowFlags());
  ~ObjectTableDialog ();
  
  virtual bool checkDuplicate (QString) const Q_DECL_OVERRIDE;
  virtual bool verify (QString) const Q_DECL_OVERRIDE;
  virtual int  drop (QString) Q_DECL_OVERRIDE;
  virtual void insert (QString) Q_DECL_OVERRIDE;
};

