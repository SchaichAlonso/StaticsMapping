#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

#include "ObjView.hpp"


namespace Widgets
{
  struct ObjScreen : QWidget
  {
    ObjScreen(QWidget * = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~ObjScreen();
    
    void setModel(Classification::ObjectPointer, QSharedPointer<VisualModel>);
    
  public Q_SLOTS:
    void setLod(int);
    void update();
    
  protected:
    void createGui();
    QList<ObjView *> createViews();
    QSlider *createSlider();
    
  protected:
    QList<ObjView *> m_views;
    QSlider *m_slider;
    
    Q_OBJECT;
  };
}
