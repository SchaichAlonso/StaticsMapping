#pragma once

#include <QtCore/QMetaEnum>
#include <QtWidgets/QComboBox>

struct EnumPicker : QComboBox
{
  EnumPicker (QMetaEnum);
  virtual ~EnumPicker ();
};
