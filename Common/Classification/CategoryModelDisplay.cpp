#include "Constants.hpp"
#include "CategoryModelDisplay.hpp"



Classification::CategoryModelDisplay::CategoryModelDisplay (QObject *o)
  : QAbstractListModel (o)
{
}



Classification::CategoryModelDisplay::~CategoryModelDisplay ()
{
}



QModelIndex
Classification::CategoryModelDisplay::index (int row, int column, const QModelIndex &parent) const
{
  QModelIndex retval;
  
  Q_UNUSED (parent);
  
  if ((row < 0) || (column < 0)) {
    return (retval);
  }
  
  retval = createIndex (row, column, Q_NULLPTR);
  
  return (retval);
}



int
Classification::CategoryModelDisplay::columnCount (const QModelIndex &parent) const
{
  Q_UNUSED (parent);
  
  return (1);
}



int
Classification::CategoryModelDisplay::rowCount (const QModelIndex &parent) const
{
  Q_UNUSED (parent);
  
  return (Constants::numberOfCategoryTypes());
}



QVariant
Classification::CategoryModelDisplay::data (const QModelIndex &mi, int role) const
{
  Constants::Category category;
  
  category = (Constants::Category) mi.row ();
  
  switch (role) {
    case Qt::UserRole:
      return (category);
    case Qt::DisplayRole:
      return (Constants::category(category));
  }
  
  return (QVariant());
}

Qt::ItemFlags
Classification::CategoryModelDisplay::flags (const QModelIndex &mi) const
{
  Q_UNUSED (mi);
  return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}
