#include "GraphBackend.hpp"

double GraphBackend::g_default_resolution = 72.0;

Agraph_t*
GraphBackend::agopen (QString name, Agdesc_t desc)
{
  Agraph_t *retval;
  QByteArray name_utf8;
  
  name_utf8 = name.toUtf8();
  
  retval = ::agopen (name_utf8.data(), desc, Q_NULLPTR);
  
  return (retval);
}

QString
GraphBackend::agget (void *object, QString attr, QString alt) const
{
  QString retval;
  QByteArray attr_utf8;
  char *ptr;
  
  attr_utf8 = attr.toUtf8 ();
  
  ptr = ::agget(object, attr_utf8.data ());

  if (ptr != Q_NULLPTR) {
    retval = ptr;
  } else {
    retval = alt;
  }
  
  return (retval);
}

int
GraphBackend::agset (void *object, QString key, QString value)
{
  QByteArray k, v;
  int retval;
  
  k = key.toUtf8 ();
  v = value.toUtf8 ();
  
  retval = ::agsafeset (object, k.data(), v.data(), v.data());
  
  return (retval);
}

Agnode_t *
GraphBackend::agnode (Agraph_t *graph, QString const& name)
{
  Agnode_t *retval;
  QByteArray name_utf8;
  
  name_utf8 = name.toUtf8();
  
  retval = ::agnode (graph, name_utf8.data(), 1);
  
  //agset (retval, "fixedsize", "true");
  agset (retval, "margin", "0");
  agset (retval, "label", name);
  agset (retval, "regular", "true");
  agset (retval, "width", "0,01");
  
  return (retval);
}









GraphBackend::GraphBackend (const QFont &font, int dpi)
  : m_context (gvContext())
  , m_graph (agopen("Hello World", Agstrictundirected))
{
  agset (m_graph, "overlap", "vpsc");
  agset (m_graph, "splines", "true");
  //agset (m_graph, "pad", "0,2");
  agset (m_graph, "dpi", QString::number(dpi));
  agset (m_graph, "nodesep", "+1");
//  agset (m_graph, "K", "1");
//  agset (m_graph, "maxiter", "1000");
//  agset (m_graph, "outputorder", "edgesfirst");
  
  qDebug ("set up %d dpi", dpi);
  
  setFont (font);
}

GraphBackend::~GraphBackend ()
{
  gvFreeLayout (m_context, m_graph);
  agclose (m_graph);
  gvFreeContext (m_context);
}



void
GraphBackend::addNode (const QString &name)
{
  m_nodes[name] = agnode (m_graph, name);
}


void
GraphBackend::addEdge (const QString &src, const QString &dst)
{
  Nodes::ConstIterator si, di, e;
  
  si = m_nodes.constFind (src);
  di = m_nodes.constFind (dst);
  e  = m_nodes.constEnd ();
  
  Q_ASSERT (si != e);
  Q_ASSERT (di != e);
  
  QPair<QString, QString> key(src, dst);
  
  if (!m_edges.contains(key)) {
    Agedge_t *edge;
    
    edge = agedge (m_graph, *si, *di, "", 1);
    
    m_edges.insert (key, edge);
  }
}


void
GraphBackend::setFont (const QFont &font)
{
  agset (m_graph, "fontname", font.family());
  agset (m_graph, "fontsize", QString("%1").arg(font.pointSizeF()));
}



void
GraphBackend::applyLayout (const QString &engine)
{
  //gvFreeLayout(m_context, m_graph);
  //gvLayout (m_context, m_graph, "sfdp");
  gvLayout (m_context, m_graph, qUtf8Printable (engine));
}

QRectF
GraphBackend::boundingRect() const
{
  boxf bb;
  
  bb = GD_bb (m_graph);
  
  qreal factor = resolution() / g_default_resolution;
  
  return QRectF(factor*bb.LL.x, factor*bb.LL.y,
                factor*bb.UR.x, factor*bb.UR.y);
}



QList<Node>
GraphBackend::nodes () const
{
  QList<Node> retval;
  boxf bb;
  double dpi, factor;
  
  dpi = resolution ();
  factor = dpi / g_default_resolution;
  
  bb = GD_bb (m_graph);
  
  for (Nodes::ConstIterator i=m_nodes.begin(), e=m_nodes.constEnd(); i!=e; ++i) {
    Agnode_t *node;
    Node object;
    pointf coord;
    double w, h;
    
    node = i.value();
    
    object.label = ND_label(node)->text;

    coord = ND_coord (node);
    
    h = ND_height (node);
    w = ND_width (node);
    
    object.center = scaled (bb, factor, coord.x, coord.y);
    
    object.dimensions.setWidth (w * g_default_resolution / factor / factor);
    object.dimensions.setHeight (h * g_default_resolution / factor / factor);
    
    retval.append (object);
  }
  
  return (retval);
}


QList<Edge>
GraphBackend::edges() const
{
  QList<Edge> retval;
  double dpi, factor, bby;
  boxf bb;
  
  dpi = resolution ();
  factor = dpi / g_default_resolution;
  bb = GD_bb (m_graph);
  bby = bb.UR.y;
  
  for (Edges::ConstIterator i=m_edges.constBegin(), e=m_edges.constEnd(); i!=e; ++i) {
    Agedge_t *edge;
    Edge object;
    Curve c;
    splines *spl;

    object.src = i.key().first;
    object.dst = i.key().second;
    
    edge = i.value ();
    
    spl = ED_spl (edge);

    if ((spl->list!=0) && (spl->list->size%3 == 1)) {
      
      pointf *ep, *li, *sp;
      
      ep = &spl->list->ep;
      li = &spl->list->list[0];
      sp = &spl->list->sp;
      
      /*
       * start and end items need special processing.
       */
      if (spl->list->sflag) {
        object.start = scaled (bb, factor, sp->x, sp->y);
        
        c = Curve (scaled (bb, factor, li->x, li->y));
        object.curves.append (c);
        
      } else {
        object.start = scaled (bb, factor, li->x, li->y);
      }
      
      for (int i=1; i<spl->list->size; i+=3) {
        c = Curve (
            scaled (bb, factor, li[i+0].x, li[i+0].y),
            scaled (bb, factor, li[i+1].x, li[i+1].y),
            scaled (bb, factor, li[i+2].x, li[i+2].y)
        );
        object.curves.append (c);
        
        if (spl->list->eflag) {
          c = Curve (scaled (bb, factor, ep->x, ep->y));
          object.curves.append (c);
        }
      }

      retval.append (object);
    }
  }

  return (retval);
}


double
GraphBackend::resolution () const
{
  QString dpi;
  double  retval;
  
  dpi = agget (m_graph, "dpi", "96,0");
  dpi.replace (",", ".");
  
  retval = dpi.toDouble ();
  return (retval);
}



QPointF
GraphBackend::scaled (const boxf &bb, double factor, double x, double y)
{
  QPointF retval;
  
  retval.setX (factor * (x - bb.LL.x));
  retval.setY (factor * (bb.UR.y - y));
  
  return (retval);
}
