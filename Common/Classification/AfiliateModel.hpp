#include "Classification.hpp"
#include "QObjectListModel.hpp"

namespace Classification
{
  struct AfiliateModel : QObjectListModel
  {
    AfiliateModel(Afiliations *);
   ~AfiliateModel();
    
    virtual int column (int) const Q_DECL_OVERRIDE;
    virtual int rowCount (const QModelIndex &) const Q_DECL_OVERRIDE;
    virtual QObjectPointer record (int row) const Q_DECL_OVERRIDE;
    
  protected:
    Afiliations *m_afiliates;
  };
}
