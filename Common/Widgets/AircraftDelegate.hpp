#include <QtWidgets/QAbstractItemDelegate>

#include <Common/Classification/Definitions.hpp>

#include "ListDelegate.hpp"

namespace Widgets
{
  struct AircraftDelegate : ListDelegate
  {
    AircraftDelegate (Classification::DefinitionsPointer);
    ~AircraftDelegate ();
    
    virtual QString displayString (const QModelIndex &index) const;
    
  protected:
    Classification::DefinitionsPointer m_definitions;
  };
}
