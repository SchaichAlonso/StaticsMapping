#include <Common/Classification/Airline.hpp>

#include "AirlineGraphWidget.hpp"
#include "GraphBackend.hpp"



AirlineGraphWidget::AirlineGraphWidget (QWidget *parent, Qt::WindowFlags flags)
  : GraphWidget (parent, flags)
{
  Q_ASSERT (layoutEngines().contains(defaultLayoutEngine()));
}



AirlineGraphWidget::~AirlineGraphWidget()
{
}



void
AirlineGraphWidget::layout (Classification::Definitions *definitions, QString engine)
{
  Q_ASSERT (layoutEngines().contains(engine));
  
  GraphBackend gb (font(), logicalDpiX());
  
  Classification::Definitions::Airlines airlines (definitions->airlines ());
  
  Q_FOREACH (Classification::AirlinePointer i, airlines) {
    gb.addNode (i->icao ());
  }
  
  Q_FOREACH (Classification::AirlinePointer i, airlines) {
    
    QString parent = i->parent ();
    QString self = i->icao ();
    
    if (parent.isEmpty()) {
      continue;
    }
    
    if (self != parent) {
      gb.addEdge (self, parent);
    }
  }
  
  gb.applyLayout (engine);
  
  m_nodes = gb.nodes ();
  m_edges = gb.edges ();
  
  resetSelected ();
  
  QRectF bb;
  int w, h;
  
  bb = gb.boundingRect ();
  w = bb.width() - bb.x();
  h = bb.height() - bb.y();
  
  setMinimumWidth (w);
  setMinimumHeight (h);
  
  setMaximumWidth (w);
  setMaximumHeight (h);
}



QString
AirlineGraphWidget::defaultLayoutEngine () const
{
  return ("sfdp");
}



QStringList
AirlineGraphWidget::layoutEngines () const
{
  QStringList list;
  
  list.append ("dot");
  list.append ("neato");
  list.append ("fdp");
  list.append ("sfdp");
  
  return (list);
}
