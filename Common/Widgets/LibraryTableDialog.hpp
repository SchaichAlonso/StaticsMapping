#pragma once

#include "TableDialog.hpp"

namespace Widgets
{
  struct LibraryTableDialog : TableDialog
  {
    LibraryTableDialog(Classification::DefinitionsPointer, QWidget * = Q_NULLPTR, Qt::WindowFlags = Qt::WindowFlags());
    ~LibraryTableDialog();
    
    virtual bool checkDuplicate(QString) const Q_DECL_OVERRIDE;
    virtual bool verify(QString) const Q_DECL_OVERRIDE;
    virtual int  drop(QString) Q_DECL_OVERRIDE;
    virtual void insert(QString) Q_DECL_OVERRIDE;
  };
}
