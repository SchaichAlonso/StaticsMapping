#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>

#include "ListDelegate.hpp"


namespace Widgets
{
  ListDelegate::ListDelegate()
  : QAbstractItemDelegate()
  {
  }
  
  
  ListDelegate::~ListDelegate()
  {
  }
  
  
  void
  ListDelegate::paint(
    QPainter *painter,
    const QStyleOptionViewItem &option,
    const QModelIndex &index
  ) const
  {
    painter->save();
    
    if (option.state & QStyle::State_Selected) {
      painter->fillRect(option.rect, option.palette.highlight());
      painter->setBrush(option.palette.highlightedText());
    } else {
      painter->setBrush(option.palette.text());
    }
    
    painter->drawText(
      option.rect.x(),
      option.rect.y() + option.rect.height() - option.fontMetrics.descent(),
      displayString(index)
    );
    
    painter->restore();
  }
  
  
  QSize
  ListDelegate::sizeHint(
    const QStyleOptionViewItem &option,
    const QModelIndex &index
  ) const
  {
    return (option.fontMetrics.size(Qt::TextSingleLine, displayString(index)));
  }
}
