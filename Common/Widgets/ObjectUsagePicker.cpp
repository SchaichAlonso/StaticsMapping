#include <QtCore/QMetaEnum>

#include <Classification/Object.hpp>

#include "ObjectUsagePicker.hpp"



ObjectUsagePicker::ObjectUsagePicker ()
  : QComboBox ()
{
  QMetaEnum me = QMetaEnum::fromType<Classification::Object::Purpose>();
  
  for (int i=0; i!=me.keyCount(); ++i) {
    const char *ptr = me.key (i);
    
    Q_CHECK_PTR (ptr);
    
    addItem (QString::fromUtf8(ptr));
  }
}


ObjectUsagePicker::~ObjectUsagePicker ()
{
}
