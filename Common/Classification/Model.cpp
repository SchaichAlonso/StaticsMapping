#include "Definitions.hpp"
#include "Model.hpp"



Classification::Model::Model (
    Definitions *defs,
    const Record &dummy
)
: QAbstractItemModel ()
, m_properties (dummy.properties())
, m_sort_permutation ()
, m_sort_column (column(dummy.primaryKeyProperty()))
, m_sort_order (Qt::AscendingOrder)
, m_definitions (defs)
{
}



Classification::Model::~Model ()
{
  m_definitions->unlinkModel (this);
}



int
Classification::Model::column (const char *property) const
{
  for (int i=0; i!=m_properties.size(); ++i) {
    if (0 == qstrcmp(property, m_properties.value(i))) {
      return (i);
    }
  }
  
  throw ("no such property");
}



int
Classification::Model::columnCount (const QModelIndex &) const
{
  return (m_properties.size());
}



QVariant
Classification::Model::headerData (int section, Qt::Orientation orientation, int role) const
{
  if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole)) {
    Q_ASSERT (section < m_properties.size());
    return (QString::fromUtf8(m_properties.value(section)));
  }
  return (QAbstractItemModel::headerData (section, orientation, role));
}



QModelIndex
Classification::Model::index (int row, int col, const QModelIndex &parent) const
{
  QModelIndex retval;
  int cols = columnCount ();
  int rows = rowCount ();
  
  if ((0 <= row) and (row < rows)) {
    if ((0 <= col) and (col < cols)) {
      /*
       * lazy initialization.
       */
      if (m_sort_permutation.isEmpty()) {
        Model *me = const_cast<Model *>(this);
        me->sort ();
        return (me->index(row, col, parent));
      }
      retval = createIndex (row, col, m_sort_permutation[row]);
    }
  }
  
  return (retval);
}



QModelIndex
Classification::Model::parent (const QModelIndex &) const
{
  QModelIndex invalid;
  return (invalid);
}



Qt::ItemFlags
Classification::Model::flags (const QModelIndex &) const
{
  Qt::ItemFlags flags;
  
  flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
  
  return (flags);
}



QVariant
Classification::Model::data (const QModelIndex &index, int role) const
{
  QVariant retval;
  
  if (index.isValid()) {
    const char *property = m_properties.value (index.column(), Q_NULLPTR);
    Q_CHECK_PTR (property);
    
    int db_index = index.internalId ();
    RecordPointer rec = record (db_index);
    if (rec) {
      switch (role) {
        case Qt::UserRole:
          retval = rec->primaryKey ();
          break;
            
        case Qt::DisplayRole:
        case Qt::EditRole:
          retval = data (rec, property);
          break;
      }
    } else {
      qCritical ("Stale Pointer at %d", index.row());
    }
  }
  
  return (retval);
}



QVariant
Classification::Model::data (RecordPointer ptr, const char *property) const
{
  return (ptr->property (property));
}



bool
Classification::Model::setData (const QModelIndex &i, const QVariant &v, int r)
{
  bool ok = false;

  if (r == Qt::EditRole) {
    if (i.isValid()) {
      const char *property = m_properties.value (i.column(), Q_NULLPTR);
      Q_CHECK_PTR (property);
      
      int db_index = i.internalId ();
      RecordPointer rec = record (db_index);
      if (rec) {
        try {
          ok = setData (rec, property, v);
        } catch (...) {
          ok = false;
        }
      } else {
        qCritical ("Stale pointer at %d", i.row());
      }
    }
  }
  
  return (ok);
}



bool
Classification::Model::setData (RecordPointer ptr, const char *property, const QVariant &value)
{
  return (ptr->setProperty (property, value));
}



void
Classification::Model::notify (int row)
{
  if (row == -1) {
    sort ();
    QAbstractItemModel::beginResetModel();
    QAbstractItemModel::endResetModel();
  } else {
    Q_ASSERT (0 <= row);
    int cols = columnCount ();
    emit dataChanged (index(row, 0), index(row, cols-1));
  }
}



void
Classification::Model::sort (int column, Qt::SortOrder order)
{
  if ((column < 0) or (column >= columnCount())) {
    column = 0;
  }
  
  m_sort_column = column;
  m_sort_order  = order;
  
  sort ();
  
  QAbstractItemModel::beginResetModel();
  QAbstractItemModel::endResetModel();
}



void
Classification::Model::sort ()
{
  QMultiMap<QVariant,int> map;
  const char *property = m_properties[m_sort_column];
  for (int i=0, r=rowCount(); i!=r; ++i) {
    RecordPointer ptr = record(i);
    map.insert (data (ptr, property), i);
  }
  
  QList<int> p = map.values ();
  if (m_sort_order == Qt::AscendingOrder) {
    m_sort_permutation = p;
  } else {
    m_sort_permutation.clear ();
    for (QList<int>::reverse_iterator i=p.rbegin(); i!=p.rend(); ++i) {
      m_sort_permutation.append (*i);
    }
  }
}
