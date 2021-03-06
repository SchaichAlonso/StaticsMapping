#include <QtWidgets/QAbstractItemDelegate>

#include <Common/Classification/Definitions.hpp>

#include "ListDelegate.hpp"

namespace Widgets
{
  struct AirlineDelegate : ListDelegate
  {
    AirlineDelegate(Classification::DefinitionsPointer);
    ~AirlineDelegate();
    
    virtual QString displayString(const QModelIndex &index) const;
    
  protected:
    Classification::DefinitionsPointer m_definitions;
  };
}
