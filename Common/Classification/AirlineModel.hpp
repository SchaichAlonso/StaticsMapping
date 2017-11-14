#pragma once

#include "Model.hpp"

namespace Classification
{
  struct AirlineModel : Model
  {
    AirlineModel (Definitions *);
    virtual ~AirlineModel ();
  
    virtual int column (int) const Q_DECL_OVERRIDE;
    virtual int rowCount (const QModelIndex &) const Q_DECL_OVERRIDE;
    virtual RecordPointer record (int row) const Q_DECL_OVERRIDE;
    
    using Model::setData;
    virtual bool setData (RecordPointer, const char *, const QVariant &) Q_DECL_OVERRIDE;
    
  Q_SIGNALS:
    void structuralChange ();
    
  private:
    Q_OBJECT
  };

}
