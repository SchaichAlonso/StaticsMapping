#pragma once

#include "Model.hpp"

namespace Classification
{
  struct ObjectModel : Model
  {
    ObjectModel (Definitions *);
    virtual ~ObjectModel ();
    
    virtual int column (int) const Q_DECL_OVERRIDE;
    virtual int rowCount (const QModelIndex &) const Q_DECL_OVERRIDE;
    RecordPointer record (int row) const Q_DECL_OVERRIDE;
    
    virtual QVariant data (RecordPointer, const char *) const Q_DECL_OVERRIDE;
    virtual bool setData (RecordPointer, const char *, const QVariant &) Q_DECL_OVERRIDE;
  };
}
