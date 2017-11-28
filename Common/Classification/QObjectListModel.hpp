#pragma once

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>

#include "Classification.hpp"

namespace Classification
{
  struct QObjectListModel : QAbstractItemModel
  {
    QObjectListModel(QList<const char *> properties, int default_sort_column);
    virtual ~QObjectListModel();
    
    virtual QObjectPointer record (int) const = 0;
    virtual QVariant userData (QObjectPointer) const;
    virtual QVariant data (QObjectPointer, const char *) const;
    virtual bool setData (QObjectPointer, const char *, const QVariant &);
    
    static  int column (QList<const char *> strings, const char *string);
            int column (const char *) const;
    virtual int column (int) const = 0;
    
    virtual int columnCount (const QModelIndex & =QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant headerData (int, Qt::Orientation, int) const Q_DECL_OVERRIDE;
    virtual QVariant data (const QModelIndex &index, int role =Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual bool setData (const QModelIndex &, const QVariant &, int =Qt::EditRole) Q_DECL_OVERRIDE;
    
    virtual QModelIndex index (int, int, const QModelIndex & =QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent (const QModelIndex &) const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags (const QModelIndex &) const Q_DECL_OVERRIDE;
    virtual void sort (int column, Qt::SortOrder =Qt::AscendingOrder) Q_DECL_OVERRIDE;
    
    void notify (int);
    
  protected:
    void sort ();
    QList<const char *>  m_properties;
    
    QList<int>           m_sort_permutation;
    int                  m_sort_column;
    Qt::SortOrder        m_sort_order;
  };
}
