#pragma once

#include <QtWidgets/QDialog>

#include <Common/Widgets/VisualObject.hpp>

struct InsertObjectConfirmationDialog : QDialog
{
  InsertObjectConfirmationDialog(Widgets::VisualObjectPointer vop, QWidget *parent=Q_NULLPTR, Qt::WindowFlags flags=Qt::WindowFlags());
  virtual ~InsertObjectConfirmationDialog();
  
protected:
  Widgets::VisualObjectPointer m_vop;
};
