#include "AfiliateModel.hpp"
#include "Afiliates.hpp"


namespace Classification {

  AfiliateModel::AfiliateModel (Afiliations *afiliates)
  : QObjectListModel (afiliates->properties(), 0)
  , m_afiliates (afiliates)
  {
  }
  
  
  
  AfiliateModel::~AfiliateModel ()
  {
  }
  
  
  
  int
  AfiliateModel::column (int name) const
  {
    Afiliations::PropertyName pn = (Afiliations::PropertyName)name;
    const char *string = Afiliations::propertyByName (pn);
    return (QObjectListModel::column (string));
  }
  
  
  
  int
  AfiliateModel::rowCount (const QModelIndex &) const
  {
    return (1);
  }
  
  
  
  Classification::QObjectPointer
  AfiliateModel::record (int) const
  {
    return (QObjectPointer(m_afiliates, [](QObject*){}));
  }
}
