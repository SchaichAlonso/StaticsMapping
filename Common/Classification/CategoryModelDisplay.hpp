#pragma once

#include <QtCore/QAbstractListModel>

namespace Classification
{
  struct CategoryModelDisplay : QAbstractListModel
  {
    CategoryModelDisplay (QObject * =Q_NULLPTR);
    virtual ~CategoryModelDisplay ();
    
    virtual QModelIndex index (int, int, const QModelIndex &) const Q_DECL_OVERRIDE;
    
    virtual int columnCount (const QModelIndex &) const Q_DECL_OVERRIDE;
    virtual int rowCount (const QModelIndex &) const Q_DECL_OVERRIDE;
    
    virtual QVariant data (const QModelIndex &, int) const Q_DECL_OVERRIDE;
    
    virtual Qt::ItemFlags flags (const QModelIndex &) const Q_DECL_OVERRIDE;
    
    Q_OBJECT
  };
}
