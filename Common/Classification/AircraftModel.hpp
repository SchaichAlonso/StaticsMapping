#pragma once

#include "Model.hpp"

namespace Classification
{
  struct AircraftModel : Model
  {
    AircraftModel (Definitions *);
    virtual ~AircraftModel ();
    
    virtual int column (int) const Q_DECL_OVERRIDE;
    virtual int rowCount (const QModelIndex &) const Q_DECL_OVERRIDE;
    virtual RecordPointer record (int row) const Q_DECL_OVERRIDE;
    
    using Model::data;
    using Model::setData;
    
    virtual QVariant data (RecordPointer, const char *) const Q_DECL_OVERRIDE;
    virtual bool setData (RecordPointer, const char *, const QVariant &) Q_DECL_OVERRIDE;
  };
}
