#include <stdexcept>

#include "Definitions.hpp"
#include "Model.hpp"



Classification::Model::Model (
    Definitions *definitions,
    const Record &dummy
)
: QObjectListModel (
    dummy.properties(),
    QObjectListModel::column(dummy.properties(), dummy.primaryKeyProperty())
  )
, m_definitions (definitions)
{
}



Classification::Model::~Model ()
{
  m_definitions->unlinkModel (this);
}



QVariant
Classification::Model::userData (QObjectPointer ptr) const
{
  RecordPointer rec = ptr.dynamicCast<Record>();
  
  QVariant retval;
  if (rec) {
    retval = rec->primaryKey ();
  }
  return (retval);
}

