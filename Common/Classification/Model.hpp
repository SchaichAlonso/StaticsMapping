#pragma once

#include <QtCore/QAbstractItemModel>

#include "QObjectListModel.hpp"

namespace Classification
{
  struct Model : QObjectListModel
  {
    Model (Definitions *, const Record &);
    virtual ~Model ();
    
    virtual QVariant userData (QObjectPointer) const Q_DECL_OVERRIDE;
    
  protected:
    Definitions *m_definitions;
  };
}
