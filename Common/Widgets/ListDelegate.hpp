#pragma once

#include <QtWidgets/QAbstractItemDelegate>

#include <Common/Classification/Definitions.hpp>

namespace Widgets
{
  struct ListDelegate : QAbstractItemDelegate
  {
    ListDelegate();
    ~ListDelegate();
    
    virtual QString displayString(const QModelIndex &index) const = 0;
    
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
  };
}
