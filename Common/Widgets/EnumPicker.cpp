#include <QtCore/QMetaEnum>

#include <Classification/Object.hpp>

#include "EnumPicker.hpp"

namespace Widgets
{
  EnumPicker::EnumPicker(QMetaEnum metaenum)
  : QComboBox()
  {
    for (int i=0; i!=metaenum.keyCount(); ++i) {
      const char *ptr = metaenum.key(i);
      
      Q_CHECK_PTR (ptr);
      
      addItem(QString::fromUtf8(ptr));
    }
  }
  
  
  EnumPicker::~EnumPicker()
  {
  }
}
