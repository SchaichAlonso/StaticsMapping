#include <QtCore/QDir>
#include <QtWidgets/QHBoxLayout>

#include "ObjViewDialog.hpp"

ObjViewDialog::ObjViewDialog (
  QString prefix,
  Classification::DefinitionsPointer defs,
  Classification::ObjectPointer obj,
  QWidget *p,
  Qt::WindowFlags f)
  : QDialog (p, f)
  , m_obj (obj)
  , m_ogl_data (defs, QDir(prefix).absoluteFilePath(obj->fileName()))
  , m_ogl_view (new ObjView())
{
  QHBoxLayout *layout = new QHBoxLayout;
  
  layout->addWidget(m_ogl_view);
  
  setLayout(layout);
}


ObjViewDialog::~ObjViewDialog ()
{
}



int
ObjViewDialog::exec ()
{
  setVisible (true);
  m_ogl_view->setModel (m_obj, m_ogl_data.model);
  m_ogl_view->setPerspective ();
  
  m_ogl_view->setPitch (30);
  m_ogl_view->setYaw (135);
  
  return (QDialog::exec());
}
