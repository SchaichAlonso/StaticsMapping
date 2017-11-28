#include <stdexcept>

#include "QObjectListModel.hpp"


namespace Classification
{
  QObjectListModel::QObjectListModel (
    QList<const char *> properties,
    int default_sort_column
  )
  : QAbstractItemModel ()
  , m_properties (properties)
  , m_sort_permutation ()
  , m_sort_column (default_sort_column)
  , m_sort_order (Qt::AscendingOrder)
  {
  }
  
  
  
  QObjectListModel::~QObjectListModel ()
  {
  }
  
  
  
  int
  QObjectListModel::column (QList<const char *> strings, const char *string)
  {
    for (int i=0; i!=strings.size(); ++i) {
      if (0 == qstrcmp(string, strings[i])) {
        return (i);
      }
    }
    throw (std::runtime_error("no such property"));
  }
  
  
  
  int
  QObjectListModel::column (const char *property) const
  {
    return (column(m_properties, property));
  }
  
  
  
  int
  QObjectListModel::columnCount (const QModelIndex &) const
  {
    return (m_properties.size());
  }
  
  
  
  QVariant
  QObjectListModel::headerData (int section, Qt::Orientation orientation, int role) const
  {
    if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole)) {
      Q_ASSERT (section < m_properties.size());
      return (QString::fromUtf8(m_properties.value(section)));
    }
    return (QAbstractItemModel::headerData (section, orientation, role));
  }
  
  
  
  QModelIndex
  QObjectListModel::index (int row, int col, const QModelIndex &parent) const
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
          QObjectListModel *me = const_cast<QObjectListModel *>(this);
          me->sort ();
          return (me->index(row, col, parent));
        }
        retval = createIndex (row, col, m_sort_permutation[row]);
      }
    }
    
    return (retval);
  }
  
  
  
  QModelIndex
  QObjectListModel::parent (const QModelIndex &) const
  {
    QModelIndex invalid;
    return (invalid);
  }
  
  
  
  Qt::ItemFlags
  QObjectListModel::flags (const QModelIndex &) const
  {
    Qt::ItemFlags flags;
    
    flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    
    return (flags);
  }
  
  
  
  QVariant
  QObjectListModel::data (const QModelIndex &index, int role) const
  {
    QVariant retval;
    
    if (index.isValid()) {
      const char *property = m_properties.value (index.column(), Q_NULLPTR);
      Q_CHECK_PTR (property);
      
      int db_index = index.internalId ();
      
      QObjectPointer ptr = record (db_index);
      
      if (ptr) {
        switch (role) {
          case Qt::UserRole:
            retval = userData (ptr);
            break;
            
          case Qt::DisplayRole:
          case Qt::EditRole:
            retval = data (ptr, property);
            break;
        }
      } else {
        qCritical ("Stale Pointer at %d", index.row());
      }
    }
    
    return (retval);
  }
  
  
  
  QVariant
  QObjectListModel::userData (QObjectPointer) const
  {
    QVariant retval;
    return (retval);
  }
  
  
  
  QVariant
  QObjectListModel::data (QObjectPointer ptr, const char *property) const
  {
    return (ptr->property (property));
  }
  
  
  
  bool
  QObjectListModel::setData (const QModelIndex &i, const QVariant &v, int r)
  {
    bool ok = false;
    
    if (r == Qt::EditRole) {
      if (i.isValid()) {
        const char *property = m_properties.value (i.column(), Q_NULLPTR);
        Q_CHECK_PTR (property);
        
        int db_index = i.internalId ();
        QObjectPointer rec = record (db_index);
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
  QObjectListModel::setData (QObjectPointer ptr, const char *property, const QVariant &value)
  {
    return (ptr->setProperty (property, value));
  }
  
  
  
  void
  QObjectListModel::notify (int row)
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
  QObjectListModel::sort (int column, Qt::SortOrder order)
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
  QObjectListModel::sort ()
  {
    QMultiMap<QVariant,int> map;
    const char *property = m_properties[m_sort_column];
    for (int i=0, r=rowCount(); i!=r; ++i) {
      map.insert (data(record(i), property), i);
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
}
