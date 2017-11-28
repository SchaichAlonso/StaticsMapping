#pragma once

#include "Model.hpp"

namespace Classification
{
  struct LibraryModel : Model
  {
    LibraryModel (Definitions *);
    virtual ~LibraryModel ();
  
    virtual int column (int) const Q_DECL_OVERRIDE;
    virtual int rowCount (const QModelIndex &) const Q_DECL_OVERRIDE;
    virtual QObjectPointer record (int row) const Q_DECL_OVERRIDE;
    
    using Model::setData;
    virtual bool setData (QObjectPointer, const char *, const QVariant &) Q_DECL_OVERRIDE;
  };

}
