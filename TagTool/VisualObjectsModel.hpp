#pragma once

#include <QtCore/QList>
#include <QtCore/QAbstractItemModel>

#include <Common/Widgets/VisualObject.hpp>

struct VisualObjectsModel : QAbstractItemModel
{
  VisualObjectsModel (QList<VisualObjectPointer> *);
  virtual ~VisualObjectsModel ();
  
  virtual QModelIndex parent(const QModelIndex &) const Q_DECL_OVERRIDE;
  virtual int columnCount (const QModelIndex &) const  Q_DECL_OVERRIDE;
  virtual int rowCount (const QModelIndex &) const Q_DECL_OVERRIDE;
  
  virtual QModelIndex index (int, int, const QModelIndex &) const Q_DECL_OVERRIDE;
  virtual QVariant data (const QModelIndex &, int) const Q_DECL_OVERRIDE;
  //virtual bool setData (const QModelIndex &, const QVariant &, int) Q_DECL_OVERRIDE;
  virtual Qt::ItemFlags flags (const QModelIndex &) const Q_DECL_OVERRIDE;
  
  void reset ();
  
protected:
  QList<VisualObjectPointer> *m_objects;
};
