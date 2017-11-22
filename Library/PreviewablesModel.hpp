#pragma once

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include <Common/Classification/WeightedObject.hpp>

struct ObjectsByXPClassModel : QAbstractItemModel
{
  ObjectsByXPClassModel ();
  virtual ~ObjectsByXPClassModel ();
  
  virtual int columnCount (const QModelIndex & =QModelIndex()) const Q_DECL_OVERRIDE;
  virtual int rowCount (const QModelIndex & =QModelIndex()) const Q_DECL_OVERRIDE;
  virtual QVariant headerData (int, Qt::Orientation, int) const Q_DECL_OVERRIDE;
  virtual QVariant data (const QModelIndex &index, int role =Qt::DisplayRole) const Q_DECL_OVERRIDE;
  
  virtual QModelIndex index (int, int, const QModelIndex & =QModelIndex()) const Q_DECL_OVERRIDE;
  virtual QModelIndex parent (const QModelIndex &) const Q_DECL_OVERRIDE;
  virtual Qt::ItemFlags flags (const QModelIndex &) const Q_DECL_OVERRIDE;
  virtual void sort (int column, Qt::SortOrder =Qt::AscendingOrder) Q_DECL_OVERRIDE;
  
  void notify ();
  
  void setObjectsByXPClass (Classification::WeightedObjectsByXPClass);
  
protected:
  QVariant displayData (int r, int c) const;
  void sort ();
  
  Classification::WeightedObjectsByXPClass m_objects;
  
  QList<int>           m_sort_permutation;
  int                  m_sort_column;
  Qt::SortOrder        m_sort_order;
};

typedef QSharedPointer<ObjectsByXPClassModel> ObjectsByXPClassModelPointer;
