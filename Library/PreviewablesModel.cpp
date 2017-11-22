#include "PreviewablesModel.hpp"


ObjectsByXPClassModel::ObjectsByXPClassModel ()
  : QAbstractItemModel ()
  , m_objects ()
  , m_sort_permutation ()
  , m_sort_column (0)
  , m_sort_order (Qt::DescendingOrder)
{
}


ObjectsByXPClassModel::~ObjectsByXPClassModel ()
{
}


int
ObjectsByXPClassModel::columnCount(const QModelIndex &mi) const
{
  if (mi.isValid()) {
    return (0);
  }
  
  return (3);
}


int
ObjectsByXPClassModel::rowCount(const QModelIndex &mi) const
{
  if (mi.isValid()) {
    return (0);
  } else {
    return (m_objects.size());
  }
}


QModelIndex
ObjectsByXPClassModel::parent(const QModelIndex &) const
{
  return (QModelIndex());
}


QVariant
ObjectsByXPClassModel::headerData (int c, Qt::Orientation o, int r) const
{
  if ((o == Qt::Horizontal) and (r == Qt::DisplayRole)) {
    switch (c) {
      case 0:
        return ("Object");
      case 1:
        return ("Weight");
      case 2:
        return ("VPath");
    }
  }
  
  return (QAbstractItemModel::headerData (c, o, r));
}



QVariant
ObjectsByXPClassModel::displayData(int r, int c) const
{
  Classification::WeightedObjectsByXPClass::ConstIterator i = m_objects.constBegin();
  i += r;
  
  switch (c) {
    case 0:
      return (i.value().object->fileName());
    case 1:
      return (i.value().weight);
    case 2:
      return (i.key().name());
  }
  
  return (QVariant());
}



QVariant
ObjectsByXPClassModel::data (const QModelIndex& index, int role) const
{
  int row = index.internalId();
  if (index.isValid()) {
    
    if (role == Qt::UserRole) {
      return ((m_objects.constBegin() + row).value().object->primaryKey());
    }
    
    if (role == Qt::DisplayRole) {
      return (displayData(row, index.column()));
    }
  }
  
  return (QVariant());
}



Qt::ItemFlags
ObjectsByXPClassModel::flags (const QModelIndex &) const
{
  return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}



QModelIndex
ObjectsByXPClassModel::index (int row, int col, const QModelIndex &parent) const
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
        ObjectsByXPClassModel *me = const_cast<ObjectsByXPClassModel *>(this);
        me->sort ();
        return (me->index(row, col, parent));
      }
      retval = createIndex (row, col, m_sort_permutation[row]);
    }
  }
  
  return (retval);
}



void
ObjectsByXPClassModel::sort (int column, Qt::SortOrder order)
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
ObjectsByXPClassModel::sort ()
{
  QMultiMap<QVariant,int> map;
  for (int i=0, r=rowCount(); i!=r; ++i) {
    map.insert (displayData(i, m_sort_column), i);
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



void
ObjectsByXPClassModel::setObjectsByXPClass (Classification::WeightedObjectsByXPClass objects)
{
  m_objects = objects;
  m_sort_permutation.clear();
  
  notify();
}



void
ObjectsByXPClassModel::notify ()
{
  sort ();
  QAbstractItemModel::beginResetModel();
  QAbstractItemModel::endResetModel();
}
