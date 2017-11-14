#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

#include "GraphWidget.hpp"


Curve::Curve (const QPointF &p)
  : is_line (true)
{
  points[0] = p;
}



Curve::Curve (const QPointF &p1, const QPointF &p2, const QPointF &p3)
  : is_line (false)
{
  points[0] = p1;
  points[1] = p2;
  points[2] = p3;
}



GraphWidget::GraphWidget (QWidget* parent, Qt::WindowFlags f)
  : QWidget (parent, f)
  , m_edges ()
  , m_nodes ()
{
}



GraphWidget::~GraphWidget ()
{
}



QPointF
GraphWidget::position (QString label) const
{
  QPointF retval;
  
  Q_FOREACH (const Node &n, m_nodes) {
    if (n.label == label) {
      retval = n.center;
      break;
    }
  }
  
  return (retval);
}



void
GraphWidget::resetSelected ()
{
  for (Nodes::Iterator i=m_nodes.begin(), e=m_nodes.end(); i!=e; ++i) {
    i->selected = false;
  }
}



void
GraphWidget::setSelected (QString label, bool value)
{
  for (Nodes::Iterator i=m_nodes.begin(), e=m_nodes.end(); i!=e; ++i) {
    if (i->label == label) {
      i->selected = value;
      break;
    }
  }
}



void
GraphWidget::mousePressEvent (QMouseEvent* event)
{
  Q_FOREACH (Node const& n, m_nodes) {
    
    float dx, dy, a, b, d;
    
    dx = event->pos().x() - n.center.x();
    dy = event->pos().y() - n.center.y();
    
    a = n.dimensions.width ();
    b = n.dimensions.height ();
    
    d = (dx*dx) / (a*a) + (dy*dy) / (b*b);
    if (d <= 1) {
      emit nodeClicked (n.label);
      //QToolTip::showText (event->globalPos(), n.label);
    }
  }
}


void
GraphWidget::paintEvent (QPaintEvent *)
{
  QPainter painter (this);
  QFontMetricsF fm(font());
  QPalette pal(palette());
  QBrush orig_brush (painter.brush());
  QPen orig_pen (painter.pen());
  
  painter.setRenderHint (QPainter::Antialiasing);
  painter.setBrush(pal.base());
  painter.drawRect (0, 0, minimumWidth(), minimumHeight());
  
  painter.setBrush (orig_brush);
  
  Q_FOREACH (Edge const& e, m_edges) {
    QPainterPath path;
    
    path.moveTo (e.start);
    Q_FOREACH (Curve const& c, e.curves) {
      if (c.is_line)
        path.lineTo (c.points[0]);
      else
        path.cubicTo (c.points[0], c.points[1], c.points[2]);
    }
    
    painter.drawPath (path);
  }
  
  Q_FOREACH (Node const& n, m_nodes) {
    
    QPalette::ColorRole bg, fg, tx;
    
    fg = QPalette::Text;
    bg = n.selected? QPalette::Highlight : QPalette::AlternateBase;
    tx = n.selected? QPalette::HighlightedText : QPalette::Text;
    
    painter.setPen (pal.color(QPalette::Normal, fg));
    painter.setBrush (pal.brush (QPalette::Normal, bg));
    painter.drawEllipse (n.center, n.dimensions.width(), n.dimensions.height());
    
    double w = fm.width (n.label);
    double h = fm.descent ();
    
    painter.setPen (pal.color(QPalette::Normal, tx));
    painter.drawText (
        n.center.x()-w/2,
        n.center.y()+h,
        n.label);
  }
  
  painter.setBrush (orig_brush);
  painter.setPen (orig_pen);
  
  painter.end ();
}
