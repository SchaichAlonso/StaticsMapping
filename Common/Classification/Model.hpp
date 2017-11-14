#pragma once

#include <QtCore/QAbstractItemModel>

#include "Classification.hpp"

namespace Classification
{
  struct Model : QAbstractItemModel
  {
    Model (Definitions *, const Record &);
    virtual ~Model ();
    
    virtual RecordPointer record (int) const = 0;
    virtual QVariant data (RecordPointer, const char *) const;
    virtual bool setData (RecordPointer, const char *, const QVariant &);
    
    virtual int column (const char *) const;
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
    
    Definitions         *m_definitions;
  };
}
