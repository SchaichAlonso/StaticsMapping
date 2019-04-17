#pragma once

#include <QtWidgets/QDialog>

#include <Common/Widgets/VisualObject.hpp>

struct InsertObjectConfirmationDialog : QDialog
{
  InsertObjectConfirmationDialog(Obj8::FilePointer file, QWidget *parent=Q_NULLPTR, Qt::WindowFlags flags=Qt::WindowFlags());
  virtual ~InsertObjectConfirmationDialog();
};
