#include <QtCore/QMetaEnum>

#include <Classification/Object.hpp>

#include "EnumPicker.hpp"



EnumPicker::EnumPicker (QMetaEnum me)
  : QComboBox ()
{
  for (int i=0; i!=me.keyCount(); ++i) {
    const char *ptr = me.key (i);
    
    Q_CHECK_PTR (ptr);
    
    addItem (QString::fromUtf8(ptr));
  }
}


EnumPicker::~EnumPicker ()
{
}
