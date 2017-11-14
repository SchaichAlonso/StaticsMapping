#include "Constants.hpp"
#include "EngineModel.hpp"



Classification::EngineModelDisplay::EngineModelDisplay (QObject *o)
  : QAbstractListModel (o)
{
}



Classification::EngineModelDisplay::~EngineModelDisplay ()
{
}



QModelIndex
Classification::EngineModelDisplay::index (int row, int column, const QModelIndex &parent) const
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
Classification::EngineModelDisplay::columnCount (const QModelIndex &parent) const
{
  Q_UNUSED (parent);
  
  return (1);
}



int
Classification::EngineModelDisplay::rowCount (const QModelIndex &parent) const
{
  Q_UNUSED (parent);
  
  return (Constants::numberOfEngineTypes());
}



QVariant
Classification::EngineModelDisplay::data (const QModelIndex &mi, int role) const
{
  Constants::Engine engine;
  
  engine = (Constants::Engine) mi.row ();
  
  switch (role) {
    case Qt::UserRole:
      return (engine);
    case Qt::DisplayRole:
      return (Constants::engine(engine));
  }
  
  return (QVariant());
}

Qt::ItemFlags
Classification::EngineModelDisplay::flags (const QModelIndex &mi) const
{
  Q_UNUSED (mi);
  
  return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}
