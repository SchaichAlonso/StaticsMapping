#include <QGridLayout>

#include <Common/Common.hpp>

#include "ObjScreen.hpp"



ObjScreen::ObjScreen (QWidget *parent, Qt::WindowFlags flags)
  : QWidget (parent, flags)
{
  QGridLayout *layout = new QGridLayout ();
  
  for (size_t i=0; i!=nitems(m_views); ++i) {
    m_views[i] = new ObjView ();
    layout->addWidget (m_views[i], i/2, i%2);
  }
  
  m_views[0]->setPerspective ();
  //m_views[1]->setRotation ();
  m_views[2]->setPitch (90);
  m_views[3]->setYaw (90);
  
  m_views[1]->setWireframe (true);
  m_views[2]->setWireframe (true);
  m_views[3]->setWireframe (true);
  
  m_slider = new QSlider (Qt::Vertical);
  m_slider->setMinimum (0);
  m_slider->setMaximum (0);
  m_slider->setSingleStep(1);
  m_slider->setTickInterval(1000);
  m_slider->setTickPosition(QSlider::TicksBothSides);
  
  QObject::connect (m_slider, SIGNAL(valueChanged(int)),
                    this, SLOT(setLod(int)));
  
  layout->addWidget(m_slider, 0, 2, -1, 1);
  
  setLayout (layout);
}



ObjScreen::~ObjScreen ()
{
}



void
ObjScreen::setModel (Classification::ObjectPointer object, QSharedPointer<VisualModel> model)
{
  for (size_t i=0; i!=nitems(m_views); ++i) {
    m_views[i]->setModel (object, model);
  }
  if (model) {
    m_slider->setMaximum (model->maxLod());
  } else {
    m_slider->setMaximum (0);
  }
  
  update ();
}



void
ObjScreen::setLod (int lod)
{
  for (size_t i=0; i!=nitems(m_views); ++i) {
    m_views[i]->setLod (lod);
  }
  
  m_slider->setToolTip(QString("lod=%1").arg(lod));
}



void
ObjScreen::update ()
{
  for (size_t i=0; i!=nitems(m_views); ++i) {
    m_views[i]->update ();
  }
  
  QWidget::update ();
}



