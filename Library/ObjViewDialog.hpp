#pragma once

#include <QtWidgets/QDialog>

#include <Common/Widgets/ObjView.hpp>
#include <Common/Widgets/VisualObject.hpp>

struct ObjViewDialog : QDialog
{
  ObjViewDialog (QString prefix, Classification::DefinitionsPointer defs, Classification::ObjectPointer obj, QWidget *parent=Q_NULLPTR, Qt::WindowFlags=Qt::WindowFlags());
  virtual ~ObjViewDialog();
  
  virtual int exec() Q_DECL_OVERRIDE;
  
protected:
  Classification::ObjectPointer m_obj;
  
  VisualObject m_ogl_data;
  ObjView     *m_ogl_view;
};
