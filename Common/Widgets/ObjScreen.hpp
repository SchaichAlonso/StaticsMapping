#pragma once

#include <QtCore/QSharedPointer>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

#include "ObjView.hpp"

struct ObjScreen : QWidget
{
  ObjScreen (QWidget * = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
  virtual ~ObjScreen ();
  
  void setModel (Classification::ObjectPointer, QSharedPointer<VisualModel>);

public Q_SLOTS:
  void setLod (int);
  void update ();
  
protected:
  ObjView *m_views[4];
  QSlider *m_slider;
  
  Q_OBJECT;
};
