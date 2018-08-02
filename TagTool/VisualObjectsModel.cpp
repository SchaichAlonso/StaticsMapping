#include "VisualObjectsModel.hpp"



VisualObjectsModel::VisualObjectsModel(QList<Widgets::VisualObjectPointer> *objects)
: QAbstractItemModel()
, m_objects(objects)
{
}



VisualObjectsModel::~VisualObjectsModel ()
{
}



QModelIndex
VisualObjectsModel::parent (const QModelIndex &) const
{
  QModelIndex retval;
  
  return (retval);
}


int
VisualObjectsModel::columnCount (const QModelIndex &) const
{
  return (1);
}



int
VisualObjectsModel::rowCount (const QModelIndex &) const
{
  int retval;
  
  retval = m_objects->size ();
  
  return (retval);
}



QModelIndex
VisualObjectsModel::index (int r, int c, const QModelIndex &) const
{
  QModelIndex retval;
  
  if ((r < 0) || (c < 0)) {
    return (retval);
  }
  
  retval = createIndex (r, c, Q_NULLPTR);
    
  return (retval);
}



QVariant
VisualObjectsModel::data (const QModelIndex &index, int role) const
{
  QVariant retval;
  
  int r(index.row());
  Widgets::VisualObjectPointer ptr(m_objects->value(r));
  
  if (ptr) {
    switch (role) {
      case Qt::DisplayRole:
        retval = QString("%1 @ %2")
            .arg(ptr->data->name())
            .arg(ptr->data->fileName());
        break;
      
      case Qt::UserRole:
        retval = ptr->data->primaryKey();
        break;
    }
  }
  
  return (retval);
}



Qt::ItemFlags
VisualObjectsModel::flags (const QModelIndex &) const
{
  Qt::ItemFlags flags;
  
  flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  //flags |= Qt::ItemIsEditable;
  
  return (flags);
}



void
VisualObjectsModel::reset ()
{
  QAbstractItemModel::beginResetModel();
  QAbstractItemModel::endResetModel();
}

