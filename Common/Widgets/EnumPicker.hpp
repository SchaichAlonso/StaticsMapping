#pragma once

#include <QtCore/QMetaEnum>
#include <QtWidgets/QComboBox>

namespace Widgets
{
  struct EnumPicker : QComboBox
  {
    EnumPicker(QMetaEnum metaenum);
    virtual ~EnumPicker();
  };
}
