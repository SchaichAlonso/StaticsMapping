#include <QGridLayout>

#include <Common/Common.hpp>

#include "ObjScreen.hpp"



ObjScreen::ObjScreen (QWidget *parent, Qt::WindowFlags flags)
  : QWidget (parent, flags)
  , m_views(createViews())
  , m_slider(createSlider())
{
  createGui();
  
  m_views[0]->setPerspective ();
  //m_views[1]->setRotation ();
  m_views[2]->setPitch (90);
  m_views[3]->setYaw (90);
  
  m_views[1]->setWireframe (true);
  m_views[2]->setWireframe (true);
  m_views[3]->setWireframe (true);
}



ObjScreen::~ObjScreen ()
{
}



void
ObjScreen::setModel (Classification::ObjectPointer object, QSharedPointer<VisualModel> model)
{
  Q_FOREACH(ObjView *v, m_views) {
    v->setModel(object, model);
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
  Q_FOREACH(ObjView *v, m_views) {
    v->setLod(lod);
  }
  
  m_slider->setToolTip(QString("lod=%1").arg(lod));
}



void
ObjScreen::update ()
{
  Q_FOREACH(ObjView *v, m_views) {
    v->update();
  }
  
  QWidget::update ();
}



void
ObjScreen::createGui()
{
  QGridLayout *layout = new QGridLayout ();
  
  Q_ASSERT(m_views.size() == 4);
  for (int i=0; i!=m_views.size(); ++i) {
    layout->addWidget(m_views[i], i/2, i%2);
  }
  
  QObject::connect(
    m_slider, SIGNAL(valueChanged(int)),
    this, SLOT(setLod(int))
  );
  
  layout->addWidget(m_slider, 0, 2, -1, 1);
  
  setLayout (layout);
}



QSlider *
ObjScreen::createSlider()
{
  QSlider *slider(new QSlider(Qt::Vertical));
  slider->setMinimum(0);
  slider->setMaximum(0);
  slider->setSingleStep(1);
  slider->setTickInterval(1000);
  slider->setTickPosition(QSlider::TicksBothSides);
  return (slider);
}


QList<ObjView *>
ObjScreen::createViews()
{
  QList<ObjView *> retval;
  for(int i=0; i!=4; ++i) {
    retval << new ObjView;
  }
  return (retval);
}
